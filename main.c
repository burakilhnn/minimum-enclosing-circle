#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

//Dosyadan okunacak noktaları temsil eden struct yapısı
struct point
{
    double x;
    double y;
} coordinats[100];

//Çemberi temsil eden struct yapısı
struct circle
{
    struct point A;
    double r;
};

//İki nokta arasındaki uzaklığı döndüren fonksiyon
double distance(struct point p1, struct point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//Verilen noktanın, verilen çemberin içerisinde olup olmadığını kontrol eden fonksiyon
bool is_insideof_circle(struct circle Circle, struct point p)
{
    return (distance(Circle.A, p) <= Circle.r);

}

//3 nokta ile belirtilen bir çemberin merkezinin koordinatlarını hesaplayan ve return eden fonksiyon
struct point find_center(double x1, double y1,double x2, double y2)
{
    double c1 = x1 * x1 + y1 * y1;
    double c2 = x2 * x2 + y2 * y2;
    double c3 = x1 * y2 - y1 * x2;
    struct point Center;
    Center.x = (y2 * c1 - y1 * c2) / (2 * c3);
    Center.y = (x1 * c2 - x2 * c1) / (2 * c3);
    return Center;
}

//Verilen 3 noktayı kesen çemberi döndüren fonksiyon
struct circle circle_intersects_three(struct point p1,struct point p2,struct point p3)
{
    struct point P = find_center(p2.x - p1.x, p2.y - p1.y,p3.x - p1.x, p3.y - p1.y);
    P.x += p1.x;
    P.y += p1.y;
    struct circle newCircle;
    newCircle.A = P;
    newCircle.r = distance(P, p1);
    return newCircle;
}

//Verilen 2 noktayı kesen çemberi döndüren fonksiyon
struct circle circle_intersects_two(struct point p1,struct point p2)
{
    struct point D;
    D.x = ((p1.x+p2.x)/2.0);
    D.y = ((p1.y+p2.y)/2.0);
    struct circle newCircle2;
    newCircle2.A = D;
    newCircle2.r = distance(p1, p2)/2.0;
    return newCircle2;
}

//Çemberin parametre olarak gönderilen struck arraydaki noktaları kapsayıp kapsamadığını kontrol eden fonksiyon
bool is_circle_enclosing(struct circle C, struct point P[],int length)
{

    for(int i=0; i<length; i++)
    {
        if (!is_insideof_circle(C, P[i]))
        {
            return false;
        }

    }
    return true;
}

//Verilen bütün noktaları kapsayan en küçük çemberi bulan fonksiyon
struct circle minimum_enclosing_circle(struct point P[],int length)
{
    //Eğer P arrayi boş ise kodu sonlandırır.
    if (length == 0)
    {
        printf("Points do not exist.");
        exit(1);
    }
    //İlk başta olabildiğince noktayı kapsayabilmek için yarıçapı sonsuza yakın ve koordinatları (0,0) olan bir mec oluşturulur.
    double f = 999999999;
    struct circle mec;
    mec.A.x = 0;
    mec.A.y = 0;
    mec.r = f;
    //Döngü içinde noktaları gez ve eğer daha küçük bir mec bulursan bunu yeni mec olarak ayarla.(2 noktada kesen çember için)
    for (int i = 0; i < length; i++)
    {
        for (int j = i + 1; j < length; j++)
        {
            struct circle tmp = circle_intersects_two(P[i], P[j]);
            if (tmp.r < mec.r && is_circle_enclosing(tmp, P,length))
            {
                mec = tmp;
            }

        }
    }
    //Döngü içinde noktaları gez ve eğer daha küçük bir mec bulursan bunu yeni mec olarak ayarla.(3 veya fazla noktada kesen çember için)
    for (int i = 0; i < length; i++)
    {
        for (int j = i + 1; j < length; j++)
        {
            for (int k = j + 1; k < length; k++)
            {
                struct circle tmp = circle_intersects_three(P[i], P[j], P[k]);
                if (tmp.r < mec.r && is_circle_enclosing(tmp, P,length))
                {
                    mec = tmp;
                }
            }
        }
    }
    return mec;
}


int main()
{
    //Varsa points.txt dosyasını açar
    FILE *myFile = fopen("points.txt","r");
    if(myFile == NULL)
    {
        printf("The file does not exist.");
        exit(1);
    }
    printf("The read points from file :\n\n");
    int numbers[100];
    int a=0,j=0,z=0;
    //Dosyanın sonuna gelene kadar okur ve okuduğu sayıları numbers arrayinde tutar
    while(!feof(myFile))
    {
        fscanf(myFile,"%d",&numbers[a]);
        a++;
    }
    int b = a/2;
    if(a%2!=0)
    {
        printf("Missing number!!!");
        exit(1);
    }
    //Sayıları coordinats struct array'e atar
    for(int i=0; i<a; i+=2)

    {
        coordinats[j].x = numbers[i];
        j++;
    }
    for(int i=1; i<a; i+=2)
    {
        coordinats[z].y = numbers[i];
        z++;
    }
    for(int i=0; i<b; i++)
    {
        printf("%.2f %.2f\n",coordinats[i].x,coordinats[i].y);
    }
    fclose(myFile);
    int length1 = a/2;
    struct circle MEC;
    printf("\nCoordinates and radius of minimum enclosing circle :\n");
    MEC = minimum_enclosing_circle(coordinats,length1);
    printf("\nx = %.2f  y = %.2f  r = %.2f \n",MEC.A.x,MEC.A.y,MEC.r);
    initwindow(1366,900,"project");
    setcolor(LIGHTBLUE);
    //Koordinat düzlemini ve çizgilerini çizer
    line(684,0,684,900);
    line(0,450,1366,450);
    for(int v=1; v<=30; v++)
    {
        line(679,450-(21*v),691,450-(21*v));
        line(679,450+(21*v),691,450+(21*v));
        line(705-(21*v),444,705-(21*v),456);
        line(684+(21*v),444,684+(21*v),456);
        if(v%5==0)
        {
            line(697,450-(21*v),673,450-(21*v));
            line(697,450+(21*v),673,450+(21*v));
            line(684-(21*v),438,684-(21*v),462);
            line(684+(21*v),438,684+(21*v),462);
        }
    }
    setcolor(WHITE);
    circle((MEC.A.x*21)+684,-(MEC.A.y*21)+450,(MEC.r)*21);
    setcolor(WHITE);
    circle((MEC.A.x*21)+684,-(MEC.A.y*21)+450,2);
    line((MEC.A.x*21)+684,-(MEC.A.y*21)+450,(MEC.A.x*21)+684+(MEC.r*21),-(MEC.A.y*21)+450);
    int graph_points[b-1][2];
    for(int i=1; i<=b; i++)
    {
        graph_points[i][1]=coordinats[i-1].x;
        graph_points[i][2]=coordinats[i-1].y;
    }
    for(int v=1; v<=b; v++)
    {
        setcolor(LIGHTCYAN);
        circle((graph_points[v][1]*21)+684,-(graph_points[v][2]*21)+450,4);
        floodfill((graph_points[v][1]*21)+684,-(graph_points[v][2]*21)+450,LIGHTCYAN);
    }
    getch();
    closegraph();
    return 0;
}
