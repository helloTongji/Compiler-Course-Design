_STACK    SEGMENT    STACK    'STACK'
          DB    32766    DUP(0)
          TOS    DW    0
_STACK    ENDS
_DATA    SEGMENT 'DATA'
f    DW    ?
g    DW    ?
a    DW    ?
b    DW    ?
c    DW    ?
e    DW    ?
item    DW    ?
factor    DW    ?
_DATA    ENDS
_TEXT    SEGMENT    'CODE'
         ASSUME    CS:_TEXT,    DS:_DATA,    SS:_STACK
_program    PROC    Far
        POP    a
        POP    b
        MOV    factor,  0
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    program_i,  EBX
program_if1_1:
        MOV    EAX,program_a
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    EAX,program_b
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        MOV    EAX,program_c
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        POP    EBX
        ADD    EBX,  item
        MOV    factor,  EBX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    ECX,  item
        CMP    EBX,  ECX
        JG    program_if1_2
        JMP    program_if1_3
program_if1_2:
        MOV    EAX,program_a
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        MOV    EAX,program_b
        MOV    factor,  EAX
        MOV    EAX,  factor
        PUSH    EAX
        MOV    EAX,program_c
        MOV    factor,  EAX
        POP    EAX
        MUL    factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        MOV    factor,  1
        MOV    EAX,  factor
        MOV    item,  EAX
        POP    EBX
        ADD    EBX,  item
        MOV    factor,  EBX
        MOV    EAX,  factor
        MOV    item,  EAX
        POP    EBX
        ADD    EBX,  item
        MOV    program_j,  EBX
        JMP    program_if1_4
program_if1_3:
        MOV    EAX,program_a
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    program_j,  EBX
program_if1_4:
program_while1_1:
        MOV    EAX,program_i
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    factor,  100
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    ECX,  item
        CMP    EBX,  ECX
        JLE    program_while1_2
        JMP    program_while1_3
program_while1_2:
        MOV    EAX,program_j
        MOV    factor,  EAX
        MOV    EAX,  factor
        PUSH    EAX
        MOV    factor,  2
        POP    EAX
        MUL    factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    program_i,  EBX
        JMP    program_while1_1
program_while1_3:
        PUSH    EBX
        RET
_program    ENDP
_demo    PROC    Far
        POP    e
        MOV    EAX,demo_f
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        MOV    factor,  2
        MOV    EAX,  factor
        MOV    item,  EAX
        POP    EBX
        ADD    EBX,  item
        MOV    demo_f,  EBX
        PUSH    EBX
        RET
_demo    ENDP
start:  MOV    AX,  _DATA
        MOV    DS,  AX
        CLI
        MOV    AX,  _STACK
        MOV    SS,  AX
        MOV    SP,  Offset  TOS
        STI
        MOV    factor,  3
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    main_a,  EBX
        MOV    factor,  4
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    main_b,  EBX
        MOV    factor,  2
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    main_c,  EBX
        MOV    EAX,main_a
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        MOV    EAX,main_b
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        MOV    EAX,main_c
        MOV    factor,  EAX
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        CALL    _demo
        POP    factor
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        PUSH    EBX
        CALL    _program
        POP    factor
        MOV    EAX,  factor
        MOV    item,  EAX
        MOV    EBX,  item
        MOV    main_a,  EBX
        MOV    AX,  4C00h
        INT    21h
_TEXT    ENDS
        END    start
