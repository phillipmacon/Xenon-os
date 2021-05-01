.code64
.intel_syntax noprefix
.global loadIdt
loadIdt:
    lidt [rdi]
    ret