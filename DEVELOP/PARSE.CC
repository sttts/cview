#include <stdio.h>
#include <string.h>
#include "v_save.h"
#include "v_load.h"
#include "v_const.h"


//===============================================


int main()
{
  char s[3];
  s[1] = '\0';
  
  char* buffer = new char[10000];
  strcpy(buffer,"");

  FILE* stream = fopen("build.cv","rt");
  if(stream!=NULL)
  {
    while(!feof(stream))
    {
      *s = (char)fgetc(stream);
      strcat(buffer,s);
    }
    fclose(stream);

    fprintf(stderr,"%s\n",buffer);

    buffer = CleanupSerializeInfo(buffer);
    fprintf(stderr,"\nCleaned up---\n%s\n",buffer);

    char* stuff = buffer;
    char* temp = ReadTag(&stuff);
    fprintf(stderr,"\nTags are---\n");
    while(temp)
    {
      fprintf(stderr,"%s\n",temp);
      temp = ReadTag(&stuff);
    }
  }
  delete buffer;
}
