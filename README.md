# minishell
[subject](https://github.com/tozggg/minishell/blob/master/subject.pdf)  
bash와 같은 쉘 스크립트 프로그램 구현하기 (with [@hoikii](https://github.com/hoikii))
<br>
<br>
## Details
### 구현요소

- echo ( -n 옵션 포함 )
- cd ( 상대 + 절대 경로 )
- pwd
- export
- unset
- env
- exit
- ! 리다이렉션 ( < > << >> )
- 파이프 ( | )
- 환경변수 ( $ )
- $?
- ctrl-C ctrl-D ctrl-\ ( 상호작용 가능할때와 안할때 구분 )

### 예외처리

- ! 오류 출력메세지와 오류코드
- 닫히지 않는 따옴표, 쓸수 없는 특수문자 처리
- 전역변수는 최대 1개
- history 저장 기능

### 허용함수

```c
--- 기본함수
#include <stdlib.h>
malloc free exit

#include <unistd.h>
read write

#include <fcntl.h>
open close

#include <stdio.h>
printf

#include <errno.h>
오류시 errno 에 해당 오류번호 삽입

#include <string.h>
char* strerror(int errno);
- 해당 에러번호의 오류메세지 리턴

#include <signal.h>
int kill(pid_t pid, int sig);
- pid에 시그널 보내기
void (*signal(int signum, void (*handler)(int)))(int);
- signum 들어오면 handler 함수 실행

--- readline 관련
#include <readline/readline.h>
#include <readline/history.h>

char *readline (const char *prompt);
- prompt를 출력하고 입력된 문자열을 리턴 ($>느낌?)
- 문자열은 개행이 제거 / 빈문자열이면 빈문자열 리턴 / 입력없을시 NULL 리턴
- ! 리턴값은 free 해줘야함

int rl_on_new_line();
void rl_replace_line (const char *text, int clear_undo);
void rl_redisplay (void);

void add_history (char *string);
- string을 history에 넣기
- 방향키로 확인가능?

--- 프로세스 관련
#include <unistd.h>

int pipe(int pipefd[2]);
- 프로세스간 통신 파이프 생성
- pipefd[0]=읽기용 pipefd[1]=쓰기용
- 성공시 0 리턴

pid_t fork(void);
- 메모리를 그대로 복사해서 자식프로세스 생성
- 성공시 부모에는 자식pid리턴, 자식에는 0리턴

#include <sys/wait.h>

pid_t wait(int *wstatus);
- 자식프로세스 종료대기 / 종료된상태면 반환 / 자식이없다면 에러
- wstatus는 자식프로세스가 종료될때의 상태 (정상?비정상)

pid_t waitpid(pid_t pid, int *wstatus, int options);
- wait에 옵션,자식골라넣기
- 위와 마찬가지로 성공시 해당 자식프로세스의 pid 리턴

--- 폴더 관련
#include <unistd.h>

char *getcwd(char *buf, size_t size);
- 현재 작업폴더의 절대경로를 size만큼 buf에 복사

int chdir(const char *path);
- 현재 작업경로를 path로 변경

#include <dirent.h>

DIR *opendir(const char *name);
- name 디렉토리의 디렉토리스트림 반환
- ? 절대경로 상대경로 둘다되나

struct dirent *readdir(DIR *dirp);
- opendir로 연 디렉토리를 읽어서 정보가 담긴 dirent 구조체 반환

int closedir(DIR *drip);
- 디렉토리 닫기 / 성공시 0

--- 파일 관련
#include <sys/stat.h>

int stat(const char *path, struct stat *buf);
- path의 파일에 대한 검색해서 stat구조체인 buf에 복사

int lstat(const char *path, struct stat *buf);
- 파일이 심볼릭링크가 아닐시 stat함수와 동일한 기능
- 심볼릭링크라면 파일이 아닌 링크에 대한 정보

int fstat(int fd, struct stat *buf);
- stat함수의 path가 fd로 변경

#include <unistd.h>

int unlink(const char *pathname);
- pathname의 파일 삭제(하드링크제거?)

int execve(const char *pathname, char *const argv[], char *const envp[]);
- pathname 실행파일 실행
- argv=해당프로그램의 인수 envp=해당프로그램의 환경변수
- 호출프로세스의 heap, stack, data 영역 초기화
- argv와 envp는 NULL로 끝나는 배열

--- 파일디스크립터 관련
#include <unistd.h>

int dup(int oldfd);
- 파일 디스크립터 복제생성 반환
- 보유하는 fd 중 가장 낮은수로

int dup2(int oldfd, int newfd);
- oldfd를 newfd에 덮어쓰기

--- 터미널 관련
#include <unistd.h>

int isatty(int fd);
- 터미널과 fd가 연결되어있는지 확인
- 연결시 1 리턴

char *ttyname(int fd);
- fd에 연결된 터미널이름 리턴

int ttyslot(void);
- 함수 호출한곳에서 사용중인 터미널의 인덱스를 리턴, 실패시 0 리턴

#include <termios.h>

int tcgetattr(int fildes, struct termios *termios_p);
- fildes라는 터미널 인덱스를 가진 터미널의 정보를 termios_p 구조체에 저장
- 성공시 0 리턴

int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p);
- fildes 터미널 속성 변경
- optional_actions = 언제 변경?
- termios_p = 변경할 속성 구조체

#include <term.h>

int tgetent(char *bp, const char *name);
int tgetflag(char *id);
int tgetnum(char *id);
char *tgetstr(char *id, char **area);
char *tgoto(const char *cap, int col, int row);
int tputs(const char *str, int affcnt, int (*putc)(int));

--- 환경변수 관련
#include <stdlib.h>

char *getenv(const char *name);
- name의 환경변수의 값을 리턴

--- 제어 관련
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```
