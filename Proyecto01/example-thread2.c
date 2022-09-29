#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//another example: https://www.youtube.com/watch?v=HDohXvS6UIk&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=7

void* routine(){
    printf("Test Thread \n");
	sleep(3);
	printf("Ending thread \n");
}
 
int main()
{
    int i;
    pthread_t t1, t2;
	
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	//loop threads
	pthread th[4];
	for(i = 0; i < 4; i++){
		pthread_create(th + i, NULL, &routine, NULL);
	}
	
    return 0;
}