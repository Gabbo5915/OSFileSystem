#include "OS.h"
int close(int fd);
int open_path(char *dirname);
int FileSubstr(char *str);
/*--------------Global Variable-------------------------*/
extern char* myvhard;//init address for virtual disk
extern string currentdir;//current direction
extern string cmd; //read commend
extern USEROPEN openfilelist[MAXOPENFILE];
extern USEROPEN *ptrcuridr;//position of current folder in openfilelist
extern DISK* disk;//struct for content
extern char command[50];//file name
/*--------------------------delete file---------------------*/
int rm(char *filename){
/*--------------------------open parent direction---------------------*/
    char newdir[20];
    char dirpath[60];
    USEROPEN *tempp = ptrcuridr;//temp store the value of ptrcuridr, for later use
    int k=FileSubstr(filename),fd=-1;
    if(k!=-1) {
        filename[k] = 0;
        memset(newdir, 0, sizeof(newdir));
        memset(dirpath, 0, sizeof(dirpath));
        strcpy(newdir, filename + k + 1);
        strcpy(dirpath, filename);
        fd = open_path(dirpath);
        if(fd!=-1) {
            ptrcuridr = openfilelist + fd;
            filename = newdir;
        }
        else {
            printf("error\n");
            return -1;
        }
    }
    //if exist file in this direction
    int BlockDirNum = (ptrcuridr->fcb).first;
    DirFile *dir = (DirFile *) disk->Data[BlockDirNum - 8];
    int temp = -1;
    for (int i = 0; i < FCBCOUNT; i++) {
        if (dir->fcb[i].free == 1 && dir->fcb[i].attribute == 0 &&
            strcmp(dir->fcb[i].filename, filename) == 0)//is file and same name
        {
            temp = i;//file exit
            break;
        }
    }
    //not exist for the direction you want to delete
    if(temp == -1) {
        printf("rm: failed to remove '%s': No such file or directory\n",filename);
        if(fd!=-1)
            close(fd);
        ptrcuridr=tempp;
        return 0;
    }
    //check if the file is open, then close it.
    char buffer[80];
    memset(buffer,0,sizeof(buffer));
    strcat(strcat(strcat(buffer,ptrcuridr->dir),"/"),ptrcuridr->fcb.filename);
    for(int i=1;i<MAXOPENFILE;i++) {
        if ((openfilelist[i].topenfile == 1 && strcmp(openfilelist[i].fcb.filename, filename) == 0 &&
             strcmp(openfilelist[i].dir,buffer) ==0 ))
        {
            printf(" The file  been opened,Now Close it !\n");
            close(i);
            break;
        }
    }
    //recycle disk
    int TEMP=0;
    for(int p=dir->fcb[temp].first;p!=-1;p=TEMP)
    {
        TEMP=disk->FAT1[p];
        disk->FAT1[p]=disk->FAT2[p]=0;
    }
    //empty direction,free:0,
    memset(&(dir->fcb[temp]),0,sizeof(dir->fcb[temp]));
    //revise length, fcbstate:1
    ptrcuridr->fcbstate=1;
    ptrcuridr->fcb.length--;//not include '.'and'..'
    /*-----------------restore-------------*/
    if(fd!=-1)
        close(fd);
    ptrcuridr=tempp;
    return 1;
}