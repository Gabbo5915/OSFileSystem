//
// Created by Administrator on 2016-06-03.
//
#ifndef A_H_
#define A_H_
#include <cstdio>
#include <memory.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
/*Const*/
#define  Path  "/home" //Root
#define BLOCKSIZE  1024
#define BLOCKCOUNT  1000
#define MAXOPENFILE 10       //maximum numbers of files to open
#define DISKSIZE  (BLOCKSIZE*BLOCKCOUNT)
#define END -1

/*-------------File Control Block -------------------*/

struct FCB
{
    char filename[12];
    char attribute;//0:folder,1:file
    int time;//create time
    int data;//create data
    int first;//init(first) disk
    int length;
    char free;//folder is empty?
};
const int FCBCOUNT = BLOCKSIZE/sizeof(FCB);//Max numebr of FCB for disk

/*-----------------Direction File---------------------*/
struct DirFile{
    FCB fcb[FCBCOUNT];  //file control block
    void init(int father,int self)
    {
        //create'..' and '.' for root.  0for".", 1for".."
        memset(fcb,0,sizeof(fcb));
        fcb[1].free=fcb[0].free=1;
        fcb[1].attribute=fcb[0].attribute=1;
        fcb[1].first=father;
        fcb[0].first=self;
        memcpy(fcb[0].filename,".",sizeof("."));
        memcpy(fcb[1].filename,"..",sizeof(".."));

    }
};

/*------------------disk------------------------*/
struct DISK
{
    int FAT1[BLOCKSIZE];//block:0-3=FAT,
    int FAT2[BLOCKSIZE];//block:4-7=FAT2
    char Data[BLOCKCOUNT-8][BLOCKSIZE];//other:8~1000
};
/*----------------Open list for User---------------------*/
struct USEROPEN
{
    FCB fcb;
    char dir[80];//name for file direction
    int count;//position of pointer inside a file while reading or writing
    char fcbstate;//if revised content of FCB，revised:1,else:0
    char topenfile;//if user's open list have been use，YES: 1,No:0
    char fatherfilename[12];//name for parent direction
    int pos;
};


#endif