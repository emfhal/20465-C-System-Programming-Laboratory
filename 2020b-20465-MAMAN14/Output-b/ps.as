; file ps.as
.entryxxx LIST
.entry LISTX
.extern W
MAIN: add r3, LIST
LOOP: prn #48
lea W, r6
inc r6
mov r3, K
sub r1, r4
bne END
cmp K, #-6
bne &END
dec W
.entry MAIN
jmpxs &LOOP
jmp &LOOP, x
add L3, L3
END: stop
STR: .string "abcd"
STRX: .string xabcd"
LIST: .data 6, -9
.data -100
K: .data 31
LIST: .data 1, -2
.extern L3
