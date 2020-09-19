#include <stdio.h>
#include <string.h>
#include "filesystem/filesystem.c"

#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define N_BLOCKS 240				  // Number of blocks in the device
#define DEV_SIZE N_BLOCKS *BLOCK_SIZE // Device size, in bytes

int main(){

  int ret=-5;
  int ret2 =-5;
  int aux =-5;
  int ret3=-5;
  int ret4=-5;
	///////
	ret = mkFS(DEV_SIZE);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

  ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

  ret = createFile("hola.txt");
  if (ret < 0)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  ///////
  ret = openFile("hola.txt");
  if (ret ==-1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED_no_exist\n", ANSI_COLOR_RESET);
    return -1;
  }
  if (ret ==-2)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -2;
  }
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  printf("DESCRIPTOR: %d\n",ret );

  ret2 = createLn("hola.txt","simb.txt");
  if (ret2 != 0)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createLn ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createLn ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

  /////
  ret3 = openFile("simb.txt");
  if (ret3 ==-1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED_no_exist\n", ANSI_COLOR_RESET);
    return -1;
  }
  if (ret3 ==-2)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -2;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  printf("DESCRIPTOR: %d\n",ret3 );

  //////////
  ///////
  char *buffer="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque efficitur erat eu euismod euismod. Maecenas bibendum ipsum eros, fringilla ullamcorper est dapibus ut. Nunc placerat erat sit amet mauris porta rhoncus. Donec pretium tincidunt felis, ut euismod ex congue a. In augue massa, ullamcorper at efficitur eget, semper quis elit. Aliquam eget sapien et justo luctus suscipit nec eget enim. Integer id odio congue, luctus mauris vel, cursus turpis. Cras feugiat nunc eget pharetra pretium. Pellentesque vitae mauris non magna aliquam vestibulum. Proin nec dolor et lorem pretium tempus in eu nulla. Curabitur vel gravida odio. Interdum et malesuada fames ac ante ipsum primis in faucibus. Cras accumsan dui quis lorem malesuada, eu finibus odio commodo. Ut fringilla, ipsum eget consectetur maximus, velit dui scelerisque lacus, vitae posuere eros mi ac velit.Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec vel vulputate lacus, ac mattis mi. Proin dignissim, lectus sit amet tincidunt pellentesque, diam massa vulputate nisl, vel porttitor nibh lectus vel mi. Nam fringilla interdum elit, sed pulvinar lacus ornare at. Etiam bibendum ipsum eget nisi congue, eu gravida leo euismod. Vestibulum pulvinar massa tellus, et placerat elit sagittis a. Aenean et pulvinar ex. Ut sagittis, sapien id dignissim maximus, elit sapien condimentum nunc, non malesuada justo eros eu augue. Aliquam non enim eu nibh scelerisque tincidunt. Vestibulum ultricies, magna iaculis pellentesque auctor, magna lacus tempor dui, at varius ex ipsum quis sem. Proin egestas libero vitae purus efficitur placerat. Vivamus iaculis condimentum aliquet. Duis tellus risus, tristique nec enim rutrum, placerat pulvinar enim. Aenean sapien leo, interdum sed metus sit amet, fermentum rutrum sapien.Sed sed porttitor odio. Duis eget commodo sem, eu accumsan felis. Duis molestie tortor laoreet, dignissim ex non, venenatis lectus. Proin faucibus eleifend mauris non dignissim. Nulla facilisi. Suspendisse potenti. Nullam facilisis imperdiet fermentum.Nunc nec elementum tellus, porttitor elementum libero. Maecenas tristique ante diam, nec tincidunt nunc lacinia quis. Fusce bibendum, urna sit amet venenatis porttitor, justo ante hendrerit ipsum, sit amet pharetra velit diam et ante. Nam faucibus gravida dictum. Sed dui quam, dictum in mauris in, hendrerit pretium augue. Fusce eget feugiat odio. Fusce dapibus placerat massa, ut tempus urna elementum nec. Aenean sapien odio, cursus euismod pharetra in, sollicitudin ac libero.Curabitur egestas faucibus dapibus. In id odio nec quam rutrum posuere. Ut tempus, mauris vitae fringilla venenatis, nisl odio laoreet neque, at ullamcorper urna libero eget turpis. Donec pulvinar varius feugiat. In leo dolor, suscipit ut urna vulputate, placerat pharetra leo. Donec id lacus diam. Sed vehicula turpis magna, a fermentum nibh rutrum ac. Suspendisse faucibus pellentesque ante, eu faucibus ex scelerisque rhoncus. Fusce vel efficitur lectus, et interdum urna.Praesent fermentum, sem id elementum vehicula, orci magna porta est, eget interdum urna augue at sem. Ut accumsan sapien vel metus malesuada eleifend a eget elit. Praesent efficitur feugiat laoreet. Pellentesque lacinia faucibus arcu, sit amet elementum magna suscipit tempus. Suspendisse commodo odio nec porttitor posuere. Mauris sit amet mauris lacus. Aliquam et arcu metus. Aenean fermentum arcu et nisi elementum volutpat. Nulla et pellentesque tellus. nec maximus pulvinar pretium. In in luctus tellus. Donec dignissim in enim quis fermentum. Praesent pretium rhoncus lacinia. Aliquam tempus pulvinar porta. Integer dapibus eros at metus cursus posuere. Suspendisse in nisl sit amet ante luctus feugiat. Donec eget dictum velit. Aliquam erat volutpat.Etiam sit amet neque quis nisi dictum imperdiet ac sed lorem. Aenean vitae massa pharetra, egestas arcu vel, elementum velit. Fusce fermentum, lorem sed blandit ultrices, velit libero pellentesque elit, quis fringilla nulla augue vel quam. Vestibulum pellentesque nulla ut arcu eleifend pretium. Duis id pellentesque urna. Sed et pretium neque, eu placerat nunc. Donec sit amet nunc neque.Donec et orci vitae lorem volutpat interdum. Phasellus vitae ultrices mi. In eros diam, lobortis id accumsan at, porttitor in odio. Donec ac consectetur sem. Praesent ut interdum nisi. Vestibulum egestas nisl dictum, suscipit ligula et, ornare nibh. Nulla condimentum orci et nibh lobortis, a fringilla nisl sagittis. Nulla facilisi. Donec scelerisque, nisi feugiat tincidunt molestie, ex lorem commodo diam, et laoreet arcu elit quis risus. Cras eu libero bibendum, suscipit nulla non, placerat purus.Sed ornare eros non erat laoreet, nec aliquet nisi blandit. In sit amet pulvinar dui. Donec rhoncus lectus tincidunt, sollicitudin ligula nec, pharetra elit. Donec eleifend consectetur sapien a hendrerit. Nunc semper, turpis auctor aliquet scelerisque, dolor risus fermentum neque, sit amet condimentum justo est volutpat nunc. Cras semper arcu at massa gravida gravida. Sed in ultricies tellus. Ut nulla erat, aliquam et dapibus non, ultrices ac lacus. Mauris ullamcorper vestibulum cursus. Nulla facilisi. Pellentesque consequat sollicitudin felis non eleifend. Donec congue congue ipsum quis ultricies. Aenean ac augue id augue aliquam tristique id non risus. Vestibulum sodales augue hendrerit sapien fermentum feugiat. Morbi viverra velit a tortor suscipit mollis. Interdum et malesuada fames ac ante ipsum primis in faucibus.Duis orci dui, congue a sapien in, efficitur suscipit elit. Curabitur id enim vel massa ultrices varius id vel ipsum. Nulla molestie elit quis justo vehicula, ut laoreet tellus lobortis. Aenean sed sem ut ex ultrices consequat a id nisl. Sed vel quam a massa fermentum sodales. Nullam orci risus, porttitor sollicitudin justo sed, faucibus rutrum odio. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Morbi a viverra est. In ut mauris tincidunt proin. ";
   aux =writeFile(ret,buffer,strlen(buffer)+1);
  if (aux == -1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  if (aux == 0)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_BLUE, "FIN DEL FICHERO, NO SE PUEDE ESCRIBIR\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  printf("BYTES ESCRITOS: %d\n",aux );


  aux=lseekFile(ret,0,2);
  if (aux == -1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

////
  char *buffer2=(char *)malloc(strlen(buffer+1));
  aux = readFile(ret3,(void *)buffer2,strlen(buffer));
  printf("BYTES LEIDOS: %d\n", aux);
  if (aux == -1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    free(buffer2);
    return -1;
  }
  if (aux == 0)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_BLUE, "FIN DEL FICHERO, NO SE PUEDE LEER\n", ANSI_COLOR_RESET);
    free(buffer2);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  free(buffer2);


  ret2 = removeLn("simb.txt");
  if (ret2 != 0)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeLn ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeLn ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  ////

  /////
  ret4 = openFile("simb.txt");
  if (ret4 ==-1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED_no_exist\n", ANSI_COLOR_RESET);
    return -1;
  }
  if (ret4 ==-2)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -2;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  printf("DESCRIPTOR: %d\n",ret4 );


  aux =closeFile(ret);

  if (aux == -1)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closekFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

/////
  ret = unmountFS();
  if (ret != 0)
  {
    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
    return -1;
  }
  fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
  ////////

  return 0;
}
