#include <curl/curl.h>
#include <stdlib.h> //malloc
#include <stdio.h> //printf
#include <string.h> //strcpy
//#include <graphics.h> //graphics.h
#include "minilibx/mlx.h" //minilibx


//derlemek için
//gcc -Wall -Wextra -Werror -lcurl main.c

char *return_value; // global
int  return_value_size; // global

//kontrol için
void print_array(char **arr)
{
    for(int i = 0;arr[i] != NULL;i++)
    {
        printf("%s\n",arr[i]);
    }
}

void print_array2(int *arr)
{
    for(int i = 0;arr[i] != -1;i++)
    {
        printf("%d\n",arr[i]);
    }
}

struct nokta
{
    int x;
    int y;
    struct nokta *next;
};

struct satir
{
    char *satir_data;
    int satir_no;
    struct nokta *nokta_listesi;
    int sekil_sayisi;
    struct satir *next;
};

char	*strjoin(const char *s1, const char *s2)
{
	char	*str;
	size_t	i;
	size_t	j;
	size_t	x;

	if (!s1 ||!s2)
		return (NULL);
	x = (strlen(s1) + strlen(s2));
	str = malloc(sizeof(char) * x + 1);
	i = -1;
	j = -1;
	if (!str)
		return (NULL);
	while (s1[++i])
		str[i] = s1[i];
	while (s2[++j])
		str[i++] = s2[j];
	str[i] = '\0';
	return (str);
}

void liste_sona_ekleme1(struct nokta **nokta_listesi,struct nokta *nokta_node)
{
    struct nokta *s;
    
    s = *nokta_listesi;
    if(s == NULL)
    {
        *nokta_listesi = nokta_node;
        nokta_node->next = NULL;
        return;
    }
    while(s->next != NULL)
    {
        s = s->next;
    }
    s->next = nokta_node;
    nokta_node->next = NULL;
}

void liste_sona_ekleme2(struct satir **satir_listesi,struct satir *satir_node)
{
    struct satir *s;
    
    s = *satir_listesi;
    if(s == NULL)
    {
        *satir_listesi = satir_node;
        satir_node->next = NULL;
        return;
    }
    while(s->next != NULL)
    {
        s = s->next;
    }
    s->next = satir_node;
    satir_node->next = NULL;
}



size_t WriteMemoryCallback(char *buf, size_t size, size_t num_item, void *s)
{
    //for flags
    (void)s;
    //printf("size : %zu\n",size); 1
    //printf("num_item : %zu\n",num_item); 99

    size_t bytes = size * num_item;
    return_value = (char*)malloc(bytes);
    int newline_flag = 0;
    for(size_t i = 0;i<bytes;i++) // using size_t cuz -w... flag
    {
        if(buf[i] == '\n')
        {
            return_value_size++;
            newline_flag = 1;
        }
        return_value[i] = buf[i];
    }
    if(newline_flag == 1)
        return_value_size++;
    return bytes;
}

#define WIDTH 1000
#define HEIGHT 1000

struct forMLX
{
    void *mlx;
    void *win;
};

void fill_white(void *mlx,void *win)
{
    int x = 0;
    int y = 0;
    while(x < WIDTH)
    {
        y = 0;
        while(y < HEIGHT)
        {
            if(x % 10 == 0)
            {
                 mlx_pixel_put(mlx,win,x,y,0xFFFFFF);
            }
            else
            {
                if(y% 10 == 0)
                {
                    mlx_pixel_put(mlx,win,x,y,0xFFFFFF);
                }
            }
            y++;
        }
        x++;
    }
}

//what is red in hex
//0xFF0000

void nokta_ciz(void *mlx,void *win,int x, int y)
{
    int a = -4;
    int b = -4;
    while(a <= 4)
    {
        b = -4;
        while(b <= 4)
        {
            if(x+a >= 0 && y+b >= 0)
            {
                mlx_pixel_put(mlx,win,(x*10)+a,(y*10)+b,0xFF0000);
            }
            b++;
        }
        a++;
    }

}


int main(int argc,char **argv)
{
    // for flags
    (void)argc;
    (void)argv;

    printf("Welcome to our PROLAB1's first project 🥳\n");
    printf("Made by Sefa TUNCA and Ardahan AYTAN\n\n");

    CURL *curl = curl_easy_init(); // curl oturumu başlatma - sonraki freeyi kullanabilmek için bunu yukarı aldim yoksa if blokta tanımlandığından hata veriyordu

    printf("Mod seçimi;\n");
    printf("1 - Siteden data alma modu\n");
    printf("2- Kullanicidan data alma modu\n");
    int mode;
    printf("Mode :");
    scanf("%d",&mode);

    if(mode == 1)
    {
        // curl devami
        printf("CURL modu secildi\n");
        
        if(curl == NULL) // curl oturumu başlatılamadı
        {
            printf("curl failed\n");
            return 0;
        }
        //set
        curl_easy_setopt(curl, CURLOPT_URL, "http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        //perform
        CURLcode result = curl_easy_perform(curl); // transfer işlemi
        if(result != CURLE_OK)
        {
            printf("Indirme hatasi : %s\n",curl_easy_strerror(result));
            return 0;
        }
        else
        {
            printf("Indirme basarili\n");
        }
    }
    else if(mode == 2)
    {
        printf("Kullanicidan data alma modu secildi\n");

        printf("Kac satir girilecek : ");
        scanf("%d",&return_value_size);
        if(return_value_size <= 0)
        {
            printf("Yanlis satir sayisi girisi\n");
            return 0;
        }
        char user_data[100];
        int user_data_index = 0;
        return_value = (char*)malloc(sizeof(char)*2);
        return_value[0] = '-';
        return_value[1] = '\0';
        while(user_data_index < return_value_size)
        {
            printf("%d. satir : ",user_data_index+1);
            scanf("%s",user_data);
            return_value = strjoin(return_value,user_data);
            if(user_data_index != return_value_size-1)
                return_value = strjoin(return_value,"\n");
            user_data_index++;
        }

        char *t4 = strdup(return_value+1); //strjoin hatasından kaçmak için kullandığımz -'yi atıyoruz attık
        return_value = t4;
    }
    else
    {
        printf("Hatali mod girisi\n");
        return 0;
    }
    printf("%s\n",return_value);
    //printf("%d\n",return_value_size);

    
    char **ret = (char**)malloc(sizeof(char*)*(return_value_size+1));//aldığımız tek boyutlu girdiği strtok ile iki boyutlu hale getiriyoruz
    char *str = strtok(return_value,"\n");
    int i = 0;
    while(str != NULL)
    {
        ret[i] = (char*)malloc(sizeof(char)*(strlen(str)+1));
        strcpy(ret[i],str);
        ret[i][strlen(str)] = '\0';
        i++;
        str = strtok(NULL,"\n");
    }
    ret[i] = NULL;
    //print_array(ret);

    // BURASINDAN SONRA OLDUĞUNDAN PEK EMİN DEĞİLİM

    int girilecek_satir_sayisi = 0;
    printf("Datadan kaç farkli satir secilecek : ");
    scanf("%d",&girilecek_satir_sayisi);
    if((girilecek_satir_sayisi > return_value_size) || girilecek_satir_sayisi <= 0)
    {
        printf("Yanlis farklı satir numarasi girisi\n");
        return 0;
    }       

    printf("Secilecek satirlar : \n");
    int *girilen_satirlar = (int*)malloc(sizeof(int)*(girilecek_satir_sayisi+1));
    for(int i = 0;i<girilecek_satir_sayisi;i++)
    {
        girilen_satirlar[i] = -1; // -1 ile dolduruyorum
    }
    for(int i = 0;i<girilecek_satir_sayisi;i++)
    {
        printf("%d. satir no : ",i+1);
        scanf("%d",&girilen_satirlar[i]);
        if(girilen_satirlar[i] > return_value_size || girilen_satirlar[i] <= 0)
        {
            printf("Yanlis satir sayisi girisi\n");
            return 0;
        }
        if(girilecek_satir_sayisi > 1) // daha önce girilmis mi kontrolü
        {
            for(int j = 0;j<i;j++)
            {
                if(girilen_satirlar[i] == girilen_satirlar[j])
                {
                    printf("Yanlis giris - Bu karakter daha once girilmis!\n");
                    return 0;
                }
            }
        }
    }
    girilen_satirlar[girilecek_satir_sayisi] = -1; // +1'lik kisim null atilamadigindan böyle yaptim
    
    //print_array2(girilen_satirlar);

    //BURADAN DEVAM EDİLECEK - BURADAN BAŞLANILDI
    printf("Secilmesi istenilen satir sayilari sorunsuz bir sekilde alindi!\n");

    struct satir *t1 = (struct satir*)malloc(sizeof(struct satir));
 
    //satirin ilk elamanını oluştur
    // burada sanırım döngü şekli yanlış - sayıları yazdığın pointerlarla döngüye girip o sayıları alıp satırı oluşturmak lazım(-1'e kadar ilerle)
    int sf = 0;
    while(girilen_satirlar[sf] != -1)
    {
        int satir_size = girilen_satirlar[sf]-1;
        struct satir *ilk_satir = (struct satir*)malloc(sizeof(struct satir));
        ilk_satir->satir_data = (char*)malloc(sizeof(char)*(strlen(ret[satir_size])-2));
        int idx = 0;
        for(size_t i = 2;i<strlen(ret[satir_size])-1;i++)
        {
            ilk_satir->satir_data[idx] = ret[satir_size][i];
            idx++;
        }
        ilk_satir->satir_data[strlen(ret[satir_size])] = '\0';
        ilk_satir->satir_no = satir_size+1;
        ilk_satir->nokta_listesi = NULL;
        ilk_satir->next = NULL;

        int nokta_sayisi = 0;
        int idx0 = 0;
        while(ilk_satir->satir_data[idx0] != '\0')
        {
            if(ilk_satir->satir_data[idx0] == '(')
            {
                nokta_sayisi++;
            }
            idx0++;
        }
         if(nokta_sayisi == 0)
        {
            ilk_satir->nokta_listesi = NULL;
        }
        char **nokta_split = (char**)malloc(sizeof(char*)*(nokta_sayisi+1));
        char *temp = strdup(ilk_satir->satir_data);  // ana structtaki datanın      bozulmaması için strtok işleminden sonra bozuluyordu
        char *tok = strtok(temp,"(");
        int i1 = 0;
        while(tok != NULL)
        {
            size_t s = 0;
            nokta_split[i1] = (char*)malloc(sizeof(char)*(strlen(tok)));
            strcpy(nokta_split[i1],tok);
            for(;tok[s] != ')';s++)
            {
                nokta_split[i1][s] = tok[s];
            }
            nokta_split[i1][s] = '\0';

            //printf("%d - %s\n",i,nokta_split[i]);
            i1++;
            tok = strtok(NULL,"(");
        }
        nokta_split[i1] = NULL;
        //print_array(nokta_split);
        //printf("\n");
        int id = 0;
        while(id < nokta_sayisi)
        {
            int temp_x = 0;
            int temp_y = 0;
            int id_ = 0;
            struct nokta *nokta = (struct nokta*)malloc (sizeof(struct nokta));
            while(nokta_split[id][id_] != ',')
            {
                temp_x = temp_x*10 + (nokta_split[id][id_] - '0');
                id_++;
            }
            id_++;
            while(nokta_split[id][id_] != '\0')
            {
                temp_y = temp_y*10 + (nokta_split[id][id_] - '0');
                id_++;
            }
            nokta->x = temp_x;
            nokta->y = temp_y;
            liste_sona_ekleme1(&ilk_satir->nokta_listesi, nokta);
            id++;
        }
        liste_sona_ekleme2(&t1,ilk_satir);
        sf++;
    }


    struct forMLX *t2 = (struct forMLX*)malloc(sizeof(struct forMLX));
    t2->mlx = mlx_init();
    t2->win = mlx_new_window(t2->mlx,WIDTH,HEIGHT, "PRO LAB1!");
    fill_white(t2->mlx,t2->win);

    int sayici = 0;
    t1 = t1->next; // basta hep null gosteriyordu nedeni bilmiom xD
    while(t1 != NULL)
    {
        while(t1->nokta_listesi != NULL)
        {
            nokta_ciz(t2->mlx,t2->win,t1->nokta_listesi->x,t1->nokta_listesi->y);
            struct nokta *t_tutucu = malloc(sizeof(struct nokta));
            t_tutucu = t1->nokta_listesi;
            while(t_tutucu->next != NULL)
            {
                if(t_tutucu->next->x == t1->nokta_listesi->x && t_tutucu->next->y == t1->nokta_listesi->y)
                {
                    //printf("Ayni noktalar bulundu.\n");
                    t1->sekil_sayisi++;
                }
                t_tutucu = t_tutucu->next;
            }
            t1->nokta_listesi = t1->nokta_listesi->next;
        }
        //printf("\n");
        printf("%d. sekil sayisi : %d\n",sayici,t1->sekil_sayisi);
        t1 = t1->next;
        sayici++;
    }


    //mlx_hook when press x button
    
    mlx_loop(t2->mlx);
    

    
    //free
    free(return_value);
    curl_easy_cleanup(curl);
   return (0);
}