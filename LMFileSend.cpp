#include "LMFileSend.h"

LMFileSend::LMFileSend(int newfd, uint32_t ip)
{
    _pid = fork();
    _fd = newfd;
    if(_pid == 0)
    {
        process_send(newfd, ip);
        exit(0);
    }
}

void LMFileSend::process_send(int newfd, uint32_t ip)
{
    FILE *fp = fdopen(newfd, "r");
    char path[1024];
    fgets(path, sizeof(path), fp);
    path[strlen(path) - 1] = 0;
    send_file(path);
    recv(newfd, path, 1, 0);
}

void LMFileSend::send_file(char *path)
{
    path = change_cwd(path);
    popen("pwd", "r");
    int ft = file_type(path);
    FILE *fp = fdopen(_fd, "w");
    if(ft == LM_FILE_TYPE_REG)
    {
        send_reg(path, fp);
    }
    else if(ft == LM_FILE_TYPE_DIR)
    {
        send_dir(path, fp);
    }
    else
    {
        printf("do not know file type = %d\n", ft);
    }
    fprintf(fp, "%s\n", LM_FILEEOF);
    fflush(fp);
}

char *LMFileSend::change_cwd(char *filename)
{
    char realpathfile[4096];
    realpath(filename, realpathfile);
    char *pos = rindex(realpathfile, '/');
    *pos = 0;
    chdir(realpathfile);
    *pos = '/';
    *(pos - 1) = '.';
    return pos - 1;
}

int LMFileSend::file_type(char *path)
{
    struct stat file_stat;
    lstat(path, &file_stat);
    if(S_ISREG(file_stat.st_mode))
    {
        return LM_FILE_TYPE_REG;
    }
    else if(S_ISDIR(file_stat.st_mode))
    {
        return LM_FILE_TYPE_DIR;
    }
    else if(S_ISLNK(file_stat.st_mode))
    {
        return LM_FILE_TYPE_LNK;
    }
    else
    {
        return LM_FILE_TYPE_OTH;
    }
}

uint64_t LMFileSend::file_size(char *path)
{
    struct stat file_stat;
    lstat(path, &file_stat);
    return file_stat.st_size;
}

void LMFileSend::send_reg(char *path, FILE *fp)
{
    fprintf(fp, "%s\n", LM_SEPARATE);
    fprintf(fp, "%s\n", LM_REG);
    fprintf(fp, "%s\n", path);
    fprintf(fp, "%llu\n", (long long unsigned int)file_size(path));
    char buf[1024];
    FILE *rfp = fopen(path, "r");
    while(1)
    {
        int ret = fread(buf, 1, sizeof(buf), rfp);
        if(ret <= 0)
        {
            break;
        }
        ret = fwrite(buf, 1, ret, fp);
        if(ret != 1)
        {
            exit(1);
        }
    }
    fclose(rfp);
}

void LMFileSend::send_dir(char *path, FILE *fp)
{
    fprintf(fp, "%s\n", LM_SEPARATE);
    fprintf(fp, "%s\n", LM_DIR);
    fprintf(fp, "%s\n", path);
    DIR *dir = opendir(path);
    struct dirent *entry;
    char newpath[1024];
    while(entry = readdir(dir))
    {
        if(string(entry->d_name) == "." || string(entry->d_name) == "..")
        {
            continue;
        }
        sprintf(newpath, "%s/%s", path, entry->d_name);
        if(entry->d_type == DT_DIR)
        {
            send_dir(newpath, fp);
        }
        else if(entry->d_type == DT_REG)
        {
            send_reg(newpath, fp);
        }
    }
    closedir(dir);
}
