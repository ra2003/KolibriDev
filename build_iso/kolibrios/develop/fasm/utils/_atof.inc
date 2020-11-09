;-----------------------------------------------------------------------------+
; �㭪�� �८�ࠧ������ ��ப� � ����⢥���� �᫮ [ by ManHunter / PCL ]   |
;-----------------------------------------------------------------------------|
; ��ࠬ����:                                                                  |
;   lpStr - 㪠��⥫� �� ��室��� ��ப� � �ଠ� ASCIIZ                     |
;   lpResult - 㪠��⥫� �� ��६�����-�ਥ���� ���祭��                      |
; �� ��室�:                                                                  |
;   EAX = 1 - ��ப� �ᯥ譮 �८�ࠧ�����                                    |
;   EAX = 0 - ��ப� �� ����� ���� �८�ࠧ����� � �᫮                      |
;-----------------------------------------------------------------------------+
proc    string2float lpStr:DWORD, lpResult:DWORD
    ; ������� ��६����
    locals
      dot    dd ?   ; �����⥫� �� �஡��� ����
      exp    dd ?   ; �����⥫� �� ��ᯮ�����
      digit  dd ?   ; ����
    endl
 
    pusha
 
    ; �஢�ઠ ��ப� �� ����������
    mov     [digit],1
 
    mov     [exp],0
    mov     [dot],0
    mov     esi,[lpStr]
    ; ����� ��� ���� ����� ���� ⮫쪮 � ��砫�
    cmp     byte [esi],'-'
    je      @f
    cmp     byte [esi],'+'
    jne     .loc_chk_loop
@@:
    inc     esi
 
    ; ��᫥ ����� �� ����� ���� ���
    cmp     byte [esi],0
    je      .loc_chk_error
.loc_chk_loop:
    ; � ��ப� ������ ���� ���, ��ᯮ���� � �� ����� ����� �窨
    lodsb
    or      al,al
    jz      .loc_chk_complete
    cmp     al,'e'
    je      .loc_chk_exp
    cmp     al,'E'
    je      .loc_chk_exp
    cmp     al,'.'
    je      .loc_chk_dot
    cmp     al,'0'
    jb      .loc_chk_error
    cmp     al,'9'
    ja      .loc_chk_error
    jmp     .loc_chk_loop
 
.loc_chk_dot:
    ; ��窠 � ��ப� 㦥 ����?
    cmp     [dot],0
    ; ��ப� ����� �����४�� �ଠ�
    jne     .loc_chk_error
 
    ; ��ᯮ���� 㦥 ����?
    cmp     [exp],0
    ; ��ப� ����� �����४�� �ଠ�
    jne     .loc_chk_error
 
    ; �����⥫� �� �஡��� ����
    mov     [dot],esi
 
    jmp     .loc_chk_loop
 
.loc_chk_exp:
    ; ��ᯮ���� 㦥 ����?
    cmp     [exp],0
    ; ��ப� ����� �����४�� �ଠ�
    jne     .loc_chk_error
 
    ; �����⥫� �� ��砫� ��ᯮ�����
    mov     [exp],esi
 
    ; �ࠧ� ��᫥ ��ᯮ����� �� ����� ���� ���
    cmp     byte [esi],0
    je      .loc_chk_error
 
    ; ��᫥ ��ᯮ����� ����� ���� ����
    cmp     byte [esi],'-'
    je      @f
    cmp     byte [esi],'+'
    jne     .loc_chk_loop
@@:
    inc     esi
 
    ; �ࠧ� ��᫥ ����� �� ����� ���� ���
    cmp     byte [esi],0
    je      .loc_chk_error
 
    ; �஢���� ᫥���騩 ᨬ���
    jmp     .loc_chk_loop
 
.loc_chk_error:
    ; ��ப� �� ���� �᫮�
    mov     [digit],0
    jmp     .loc_ret
 
.loc_chk_complete:
    ; ���樠������ ᮯ�����
    finit
 
    ; ��砫쭮� ���祭�� �᫠
    fldz
 
    ; �����⥫� � ����⥫�
    mov     [digit],10
    fild    dword [digit]
 
    ; ������ ���祭�� �� ����⮩
    mov     esi,[lpStr]
 
    ; � ��砫� ��ப� �����?
    cmp     byte [esi],'-'
    je      @f
    cmp     byte [esi],'+'
    jne     .loc_before_dot
@@:
    inc     esi
    ; �८�ࠧ������ �᫠ �� ����⮩
.loc_before_dot:
    lodsb
    ; ����� ��ப�?
    or      al,al
    jz      .loc_complete
 
    cmp     al,'.'
    je      .loc_complete_before_dot
    cmp     al,'e'
    je      .loc_exp
    cmp     al,'E'
    je      .loc_exp
 
    ; ��।��� ���
    sub     al,'0'
    movzx   eax,al
    mov     [digit],eax
 
    ; �������
    fild    dword [digit]
    fxch    st2
    fmul    st0,st1
    fxch    st2
    fadd    st2,st0
 
    ffree   st0     ; ������� �⥪
    fincstp
 
    jmp     .loc_before_dot
 
    ; �८�ࠧ������ �஡��� ��� �᫠
.loc_complete_before_dot:
    ; �஡��� ���� ����?
    cmp     [dot],0
    je      .loc_complete_after_dot
 
    ; ��ᯮ���� ����?
    cmp     [exp],0
    je      @f
 
    ; �����⥫� �� ��砫� ��ᯮ�����
    mov     esi,[exp]
    jmp     .loc_start_after_dot
@@:
    ; ���� ��७��� 㪠��⥫� �� ����� ��ப�
    xor     ecx,ecx
    dec     ecx
    xor     eax,eax
    mov     edi,esi
    repne   scasb
 
    mov     esi,edi
 
.loc_start_after_dot:
    std
    dec     esi
    dec     esi
 
    ; �஡��� ����
    fldz
    fxch    st1
.loc_after_dot:
    lodsb
    ; ����� �஡��� ���?
    cmp     al,'.'
    je      .loc_complete_after_dot
 
    ; ��।��� ���
    sub     al,'0'
    movzx   eax,al
    mov     [digit],eax
 
    ; �������
    fild    dword [digit]
    fadd    st2,st0
    fxch    st2
    fdiv    st0,st1
    fxch    st2
 
    ffree   st0     ; ������� �⥪
    fincstp
 
    jmp     .loc_after_dot
 
.loc_complete_after_dot:
    ; ������ 䫠� ���ࠢ�����
    cld
 
    ffree   st0     ; ������� �⥪
    fincstp
 
    ; ������� �஡��� � 楫�� ����
    fadd    st1,st0
 
.loc_exp:
    ; ��ᯮ���� ����?
    cmp     [exp],0
    je      .loc_complete
 
    ; ������� ���祭�� ��ᯮ�����
    xor     ecx,ecx
 
    mov     esi,[exp]
    ; � ��砫� ��ப� �����?
    cmp     byte [esi],'-'
    je      @f
    cmp     byte [esi],'+'
    jne     .loc_start_exp
@@:
    inc     esi
.loc_start_exp:
    lodsb
    or      al,al
    jz      .loc_end_exp
 
    sub     al,'0'
    movzx   eax,al
    imul    ecx,10
    add     ecx,eax
 
    jmp     .loc_start_exp
.loc_end_exp:
 
    or      ecx,ecx
    jz      .loc_complete
 
    ffree   st0     ; ������� �⥪
    fincstp
 
    mov     [digit],10
    fild    dword [digit]
 
    ; ������ ��� 㬭�����?
    mov     esi,[exp]
    cmp     byte [esi],'-'
    je      .loc_exp_divide
 
.loc_exp_multiple:
    fmul    st1,st0
    loop    .loc_exp_multiple
    jmp     .loc_complete
 
.loc_exp_divide:
    fdiv    st1,st0
    loop    .loc_exp_divide
 
.loc_complete:
    ffree   st0     ; ������� �⥪
    fincstp
 
    ; � ��砫� ��ப� �����?
    mov     esi,[lpStr]
    cmp     byte [esi],'-'
    jne     @f
 
    ; �������� ���� �᫠
    fchs
@@:
    ; ������� ���祭�� � �祩�� �����
    mov     eax,[lpResult]
    ; �᫨ �ॡ���� ����襭��� �筮���, � �ਥ����
    ; ������ ����� ࠧ��� QWORD, � ᫥������ �������
    ; ���� �������� �� fstp qword [eax]
    fstp    tword [eax]
 
    ; �ᯥ譮� �८�ࠧ������
    mov     [digit],1
.loc_ret:
    popa
 
    ; ������� �८�ࠧ������
    mov     eax,[digit]
 
    ret
endp