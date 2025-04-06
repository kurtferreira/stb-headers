#define JOB_IMPLEMENTATION
#include "../jobs.h"
#include <stdio.h>
#include <fcntl.h>

int key_pressed(FILE *fp) {
    int flags = fcntl(fileno(fp), F_GETFL, 0);
    fcntl(fileno(fp), F_SETFL, flags | O_NONBLOCK);
    int c = fgetc(fp);
    fcntl(fileno(fp), F_SETFL, flags);
    return c;
}

typedef struct {
    const char *from;
    const char *message;
} sms_t;

void sms(void* jm, void* data)
{
    sms_t* sms = (sms_t*)data;
    printf("-> Sending sms from %s (%s)\n", sms->from, sms->message);
}

void voicemail(void* jm, void* data)
{
    printf("-> Leading voicemail for %s\n", (const char*)data);
}

void phone(void* jm, void* data)
{
    const char* phone = (const char*)data;

    jobs_t *js = (jobs_t*)jm;
    for (int i = 0; i < 10; ++i) {
        printf("-> Ring (%s)#%i\n", phone, i);
        job_yield(js, 1); // yield for other jobs
    }

    jobs_add_job(js, voicemail, (void*)phone);
}

int main(int argc, char **argv) {
    printf("] Job System Example\n");

    printf("> initializing job system...\n");
    jobs_t *jm = jobs_init(1024);

    sms_t msg1 = { "Fiora", "Hello!"};
    sms_t msg2 = { "Angela", "Hey there!"};
    sms_t msg3 = { "Martin", "Well howdy partner."};

    jobs_add_job(jm, phone, "0713560476");
    jobs_add_job(jm, sms, &msg1);
    jobs_add_job(jm, sms, &msg2);
    jobs_add_job(jm, sms, &msg3);

    printf("> press enter to quit\n");
    // keep processing jobs until we press a key
    while(key_pressed(stdin) == EOF) {
        jobs_process(jm);
    }

    printf("< freeing job system...\n");
    jobs_free(jm);

    printf("] bye.\n");
    return 0;
}
