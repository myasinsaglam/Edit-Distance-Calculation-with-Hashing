/**
@file

Bu uygulamada verilen sozlukteki kelimeler Hashing yontemiyle bir listede saklanmistir.
Test olarak verilen kelimelerden yazim hatali olanlara Levenshtein Distance mesafesi ile benzer doğru kelimeler bulunup yazım duzeltme onerisi detayli olarak gerceklestirilmistir.
Sonuclar ise dosyaya kaydedilmistir.
@author

Name 			 :		Muhammed Yasin SAGLAM	
Student No		 :	 	15011804	
Date 			 :		28/10/2017
E-Mail			 :		myasinsaglam1907@gmail.com
Compiler Used	 :		GCC
IDE			 	 :		DEV-C++(Version 5.11)
Operating System :		Windows 10 Educational Edition
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define hashFile "hash.txt" //hash dosya adini tutan makro
#define dictFile "newdict.txt"//sozluk dosya adini tutan makro
#define testFile "test.txt"//test kelimelerinin dosya adini tutan makro
#define resultFile "result.txt"//sonuclarin dosya adini tutan makro
#define test_size 100 //test list size
#define size 133798  //dictionary size
#define hash_size 267601 //hash size
#define word_len 35 //max word length
#define threshold 7 //an edit distance threshold value to determine it not has a similiar word NONE word.
int distance(char *word1,char *word2);//iki kelimenin edit distance ini hesaplayan ve degeri donduren fonksiyon
int cal_hash(char ***hashtable,char *word);//verilen kelimeyi hashtable da ilgili indise yazan fonksiyon
int save_hash(char ***hashtable);//oluşturulan hash listesini dosyaya kaydeden fonksiyon
void load_hash(char ***hashtable);//kayitli hash listesini dosyadan okuyan fonksiyon
int look_hash(char ***hashtable,char *word);//verilen kelimeyi hashtable da arayan bulursa 1 donduren fonksiyon
int save_changes(FILE **fp,char *word1,char *word2);//2 kelimenin donusumunu dosyaya yazan fonksiyon
int find_mindex(int *arr,int arr_size);//verilen dizinin en kucuk elemaninin indisini donduren fonksiyon

int main(){
    FILE *fp,*fptest,*fpresult; //dosya okuma ve yazma islemleri icin file pointerlar tanimlaniyor
    int i,j,choice; //cevrim degiskenleri ve menu secimi degiskeni
    int min_ed,mindex,dist; //fonksiyonlardan donen edit distance degerlerini ve minimum bulmak icin indislerini tutacak degiskenler
    char **tests,**words,**hashtable; //test,sozluk ve hashtable icin pointerlar tanimlaniyor
    
    words=(char**)calloc(size,sizeof(char*));	//sozlukteki kelimeler icin memory allocation
    for(i=0;i<size;i++){
        words[i]=(char*)calloc(word_len,sizeof(char));
    }
    fp=fopen(dictFile,"r"); //sozluk dosyasi aciliyor
    if(!fp){
        printf("Dictionary file not opened !!! Quitting...\n");
        exit(0);
    }
    i=0;
    while(!feof(fp)){
        fscanf(fp,"%s",words[i]); //kelimeler bellege aliniyor 
        words[i]=strlwr(words[i]); //kelimeler kucuk harfe cevriliyor
        i++;
    }
    fclose(fp);
    printf("\nDictionary read from file...\n");
    system("PAUSE");
    system("CLS");
    //hashtable dizisi olusturuluyor
    hashtable=(char**)calloc(hash_size,sizeof(char*)); //Hash tablosu icin memory allocation
    for(i=0;i<hash_size;i++){
        hashtable[i]=(char*)calloc(word_len,sizeof(char));
    }
    
    //secenek menusu
    printf("\n1.Create HashList and Save\n2.Load Hash List\n3.Test for only one input\n4.Write results to file\n\nPlease enter the choice (0 for exit): ");
    scanf("%d",&choice);					//Reading choice from the user
    system("CLS");
    while(choice!=0){
        if(choice==1){ //hash listesi olusturur ve dosyaya kaydeder
            system("CLS");
            printf("\nDictionary is hashing and saving as .txt file.\nWorking....\n");
			for(i=0;i<size;i++){
                cal_hash(&hashtable,words[i]); //cal_hash cagrilir ve hash tablosu olusturulur
            }
            save_hash(&hashtable); //hashtable dosyaya yazilir
            system("CLS");
            printf("\nDictionary hashed and saved succesfully...\n");
            system("PAUSE");
        }
        if(choice==2){ //onceden olusturulmus hashlistesini dosyadan okur.
        	system("CLS");
			load_hash(&hashtable); //hash tablosu dosyadan okunur 
			printf("\nHash list read from file succesfully...\n");
			system("PAUSE");
        }
        if(choice==3){ //kullanci tarafindan verilen 1 adet kelimeyi hash tablosunda arar bulamazsa en yakin olan kelimeyi ekrana yazdirir.
        	system("CLS");
            char *testword=(char*)calloc(word_len,sizeof(char));
            printf("Enter input to test hash functionality : ");
            scanf("%s",testword);
            testword=strlwr(testword);
            if(look_hash(&hashtable,testword)){
                //system("CLS");
				printf("\nFound in hashlist...\n");
            }
            else{
            	printf("\nGiven word is not found in hashlist...Searching similar word...\n");
                min_ed=distance(words[0],testword);
                for(i=0;i<size;i++){
                    if(distance(words[i],testword)<min_ed){
                        min_ed=distance(words[i],testword);
                        mindex=i;
                    }
                }
                //system("CLS");
                if(min_ed>=threshold){ //kelimeye en benzer kelime thresholddan fazlaysa anlamsiz girdi demektir
                	printf("\nNONE\n");
				}
				else{
					printf("\nMost similar word is %s Edit distance is %d\n",words[mindex],min_ed);
				}
            }
            system("PAUSE");
        }
        if(choice==4){
        	system("CLS");
            tests=(char**)calloc(test_size,sizeof(char*));//test edilecek kelimeler icin memory allocation
		    for(i=0;i<test_size;i++){
		        tests[i]=(char*)calloc(word_len,sizeof(char));
		    }
		
		    //test edilecek kelimelerin dosyadan tests dizisine okunuyor ve kucuk harfe cevriliyor
		    fptest=fopen(testFile,"r");
		    if(!fptest){
		        printf("Test file not opened!!! Quitting...\n");
		        exit(0);
		    }
		    for(i=0;i<test_size;i++){
		        fscanf(fp,"%s",tests[i]); //test kelimeleri okunuyor
		        tests[i]=strlwr(tests[i]); //hepsi kucuk harfe cevriliyor
		    }
		    fclose(fptest);	//dosya kapatiliyor    
		    printf("\nTest inputs read from file...\n");
		    system("PAUSE");
		    system("CLS");
        	printf("Working...\n");
            fpresult=fopen(resultFile,"w+"); //sonuclarin yazilacagi dosya aciliyor
            if(!fpresult){
                printf("\nResult file is not created!!! Quitting...\n");
                exit(0);
            }
            i=0;
            while(i<test_size){ //eger 
                min_ed=100;
                if(look_hash(&hashtable,tests[i])){ //kelime sozlukte var mi hashlistten bak 
                    fprintf(fpresult,"\n%-35s\tOK\n",tests[i]);
                }
                else{ //yoksa 
                    for(j=0;j<size;j++){ //minimum ed ye sahip olan kelimeyi bul
                        dist=distance(tests[i],words[j]);
                        if(dist<min_ed){
                            min_ed=dist;
                            mindex=j;
                        }
                    }
                    if(min_ed>=threshold){ //eger thresholdu gecememisse 
                        fprintf(fpresult,"\n%-35s\tNONE\n",tests[i]); //anlamsiz yaz dosyaya
                    }
                    else{ //gecmisse 
                        fprintf(fpresult,"\n%-35s%-35s\t",tests[i],words[mindex]); //benzerini dosyaya yaz  
                        save_changes(&fpresult,tests[i],words[mindex]); //degisiklikleri hesaplayan fonksiyonu cagirarak satiri tamamla
                    }
                }
                i++;
            }
            fclose(fpresult); //sonuc dosyasi kapatiliyor
            for(i=0;i<test_size;i++){ //test kelimeleri free
        		free(tests[i]);
    		}
    		free(tests);
            system("CLS");
			printf("\nResults saved file(results.txt) successfully...\n");
            system("PAUSE");
        }
        system("CLS");
        system("COLOR a");
        printf("\n1.Create HashList and Save\n2.Load Hash List\n3.Test for only one input\n4.Write results to file\n\nPlease enter the choice (0 for exit): ");
        scanf("%d",&choice);
    }

//PROGRAM SONU

    for(i=0;i<size;i++){ //sozluk free yapiliyor
        free(words[i]);
    }
    free(words);
    for(i=0;i<hash_size;i++){ //hashtable free yapiliyor
        free(hashtable[i]);
    }
    free(hashtable);

    system("PAUSE");
    return 0;
}


//verilen kelimeyi hashtable da arayan bulursa 1 donduren fonksiyon 
int look_hash(char ***hashtable,char *word){
    int len = strlen(word); //kelime uzunlugu okunuyor
    int i; //cevrim degiskeni
    int hash,sum=0; //hash degeri ve harflerin ascii toplamlarini tutan sum degiskeni

    for(i=0;i<len;i++){
        sum+=word[i]; //herbir harfin ascii degeri toplaniyor
    }

    hash=(sum*26)%hash_size; //hash fonsiyonu uygulanarak hash degeri hesaplaniyor

    i=0;
    while(strcmp((*hashtable)[hash],"")!=0 && i<=hash_size){ //hash de ki indiste eleman oldukca ve cevrim hash size dan az oldukca
        if(strcmp((*hashtable)[hash],word)==0){ //aranan kelime sozlukte var mi
            return 1; //bulundu anlamina gelir 1 dondurur
        }
        else{
            hash=(hash+1)%hash_size; //yoksa null gorene kadar indis ilerler
        }
        i++; //listeden kac elemana bakildi
    }
    return 0; //bulunamadi anlaminda cikis demektir eleman sozlukte yok null a denk gelmis
}

//verilen kelimeyi hashtable da ilgili indise yazan fonksiyon
int cal_hash(char ***hashtable,char *word){
    int len = strlen(word); //kelime uzunlugu okunuyor
    int i; //cevrim degiskeni
    int hash,sum=0; //hash degeri ve harflerin ascii toplamlarini tutan sum degiskeni

    for(i=0;i<len;i++){
        sum+=word[i]; //herbir harfin ascii degeri toplaniyor
    }

    hash=(sum*26)%hash_size; //hash fonsiyonu uygulanarak hash degeri hesaplaniyor

    if(strcmp((*hashtable)[hash],"")==0){ //eger tablodaki goz bossa
        strcpy((*hashtable)[hash],word); //elemani yaz
    }
    else{ //degilse 
        while(strcmp((*hashtable)[hash],"") != 0){//bos olana kadar
            hash=(hash+1)%hash_size; //dairesel bicimde ilerle
        }
        strcpy((*hashtable)[hash],word); //sonrasinda ise elemani yerlestir
    }

    return 0;
}

//oluşturulan hash listesini dosyaya kaydeden fonksiyon
int save_hash(char ***hashtable){
    FILE *fp;
    int i;
    fp=fopen(hashFile,"w");
    if(!fp){
        printf("\nError occured!!! Hash file not created to write...\n");
        exit(0);
    }
    for(i=0;i<hash_size;i++){
        fprintf(fp,"%s\n",(*hashtable)[i]);
    }
    printf("\nHash file created named hash.txt\n");
    fclose(fp);
    return 0;
}

//kayitli hash listesini dosyadan okuyan fonksiyon
void load_hash(char ***hashtable){
    FILE *fp;
    int i;
    char *temp;
    fp=fopen(hashFile,"rb+"); //hash dosyasi okuma modunda aciliyor
    if(!fp){
        printf("\nError occured!!! Hash file not opened...\n");
        exit(0);
    }
    size_t len;
    for(i=0;i<hash_size;i++){  //boşluklu yapıdaki hash dosyasından okuma yaparken oluşan karakter fazlalığı hatasınıın giderildiği kısım
        temp=(char*)malloc(sizeof(char)*50);
        fgets(temp,50,fp);
        len=strlen(temp)-2; //fazlalik olan son 2 karakteri alma
        memcpy((*hashtable)[i],temp,len); 
        free(temp);
    }
    fclose(fp);

}

//iki kelimenin edit distance ini hesaplayan fonksiyon
int distance(char *word1,char *word2){
    int len1=strlen(word1)+1; //kelime 1 uzunlugu
    int len2=strlen(word2)+1; //kelime 2 uzunlugu
    int i,j; //cevrim degiskenleri
    char c1,c2; //karakter kiyaslamasi icin degiskenler
    int deletion,insertion,change,minimum,edit_distance; //ceza islemlerinin uygulanmasini saglayan degiskenler
    //2 boyutlu dinamik distance matrisi tanimlaniyor
    int **matrix=(int**)calloc(len1,sizeof(int*));
    for(i=0;i<len1;i++){
        matrix[i]=(int*)calloc(len2,sizeof(int));
    }
    if(!matrix){
        printf("Allocation error!!! Quitting...");
        exit(0);
    }
    //matris baslangic degerleri ataniyor (initialization)
    for(i=0;i<len1;i++){
        matrix[i][0]=i;
    }
    for(i=0;i<len2;i++){
        matrix[0][i]=i;
    }
    //matris hesaplamalari
    for(i=1;i<len1;i++){
        c1=word1[i-1]; //ilk kelimenin karakteri
        for(j=1;j<len2;j++){
            c2=word2[j-1]; //ikinci kelimenin karakteri
            if(c1==c2){ //birbirlerine esitler mi?
                matrix[i][j]=matrix[i-1][j-1]; //esitse sol ustteki degeri ver copy olmus demektir ceza=0
            }
            else{//degilse
                deletion = matrix[i-1][j] + 1; //silme olmussa +1
                insertion = matrix[i][j-1] + 1; //ekleme varsa +1
                change = matrix[i-1][j-1] + 2; // degisim varsa ceza +2
                //minimum bulunur
                minimum = deletion;
                if (insertion < minimum) {
                    minimum = insertion;
                }
                else if (change < minimum) {
                    minimum = change;
                }
                matrix[i][j] = minimum; //ilgili goze minimum deger atamasi yapilir
            }
        }
    }
    edit_distance =	matrix[len1-1][len2-1];
    //free matrix
    for(i=0;i<len1;i++){
        free(matrix[i]);
    }
    free(matrix);

    return edit_distance;
}

//2 kelimenin donusumunu dosyaya yazan fonksiyon
int save_changes(FILE **fp,char *word1,char *word2){
    int len1=strlen(word1)+1; //kelime 1 uzunlugu
    int len2=strlen(word2)+1; //kelime 2 uzunlugu
    int i,j; //cevrim degiskenleri
    char c1,c2; //karakter kiyaslamasi icin degiskenler
    int deletion,insertion,change,minimum,edit_distance,mindex; //ceza islemlerinin uygulanmasini saglayan degiskenler
    int *way=(int*)calloc(3,sizeof(int)); //backtrack icin yon degerlerinin tutulacagi way adli dizi
    if(!way){
        printf("Allocation error!!! Quitting...");
        exit(0);
    }
    //2 boyutlu dinamik distance matrisi tanimlaniyor
    int **matrix=(int**)calloc(len1,sizeof(int*));
    for(i=0;i<len1;i++){
        matrix[i]=(int*)calloc(len2,sizeof(int));
    }
    if(!matrix){
        printf("Allocation error!!! Quitting...");
        exit(0);
    }
    //matris baslangic degerleri ataniyor (initialization)
    for(i=0;i<len1;i++){
        matrix[i][0]=i;
    }
    for(i=0;i<len2;i++){
        matrix[0][i]=i;
    }
    //matris hesaplamalari
    for(i=1;i<len1;i++){
        c1=word1[i-1]; //ilk kelimenin karakteri
        for(j=1;j<len2;j++){
            c2=word2[j-1]; //ikinci kelimenin karakteri
            if(c1==c2){ //birbirlerine esitler mi?
                matrix[i][j]=matrix[i-1][j-1]; //esitse sol ustteki degeri ver copy olmus demektir ceza=0
            }
            else{//degilse
                deletion = matrix[i-1][j] + 1; //silme olmussa +1
                insertion = matrix[i][j-1] + 1; //ekleme varsa +1
                change = matrix[i-1][j-1] + 2; // degisim varsa ceza +2
                //minimum bulunur
                minimum = deletion;
                if (insertion < minimum) {
                    minimum = insertion;
                }
                else if (change < minimum) {
                    minimum = change;
                }
                matrix[i][j] = minimum; //ilgili goze minimum deger atamasi yapilir
            }
        }
    }
    edit_distance =	matrix[len1-1][len2-1];


    //backtracking
    i=len1-1;
    j=len2-1;



    while(i!=0 && j!=0){ //kenarlara gelene kadar 
        way[0] = matrix[i-1][j]; //0.goz yukaridaki deger  --delete
        way[1] = matrix[i][j-1]; //1.goz soldaki deger --insert
        way[2] = matrix[i-1][j-1]; //2.goz caprazdaki deger --change degerini saklar
        mindex=find_mindex(way,3); //degeri minimum olan yonu dondurur
        if(mindex == 0){ //eger yukaridan gelinmisse 
            fprintf(*(fp)," (del->%c) ",word1[--i]); //i 1 azaltilir yani yukari ilerlenir ve test kelimesinden ilgili harf silinmeli 
        }
        else if(mindex == 1){ //eger soldan gelinmisse 
            fprintf(*(fp)," (ins->%c) ",word2[--j]); //j 1 azaltilir yani sola ilerlenir ve test kelimesine ilgili  harf eklenir
        }
        else if(matrix[i-1][j-1] != matrix[i][j]){ //eger caprazdaki deger degismisse 
            fprintf(*(fp)," (chg->%c,%c) ",word1[--i],word2[--j]); //i ve j 1 azaltilir yani capraza ilerlenir ve degisiklik yapilir
        }
        else{ //eger capraz ayniysa ve minimumsa degisiklik yok devam edilir 
            fprintf(*(fp)," %c ",word1[--i]);
            j--;
        }
    }

    while (j>0){ //ustteki kenara gelinmisse surekli sola gidilir ve ekleme yapilir
        fprintf(*(fp), " (ins->%c) ", word2[--j]);
    }

    while (i>0){ //yandaki kenara gidilmisse surekli yukari cikilir yani silme yapilir.
        fprintf(*(fp), " (del->%c) ", word1[--i]);
    }

    fputs("\n",*(fp));

    //free matrix
    for(i=0;i<len1;i++){
        free(matrix[i]);
    }
    free(matrix);

    return edit_distance;
}

//verilen dizinin en kucuk elemaninin indisini donduren fonksiyon
int find_mindex(int *arr,int arr_size){
    int i,min,mindex;
    min=arr[0];
    mindex=0;
    for(i=1;i<arr_size;i++){
        if(arr[i]<min){
            min=arr[i];
            mindex=i;
        }
    }
    return mindex;
}



