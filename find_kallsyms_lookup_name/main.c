#include <linux/kprobes.h>
#include <linux/printk.h>
#include <linux/module.h>

typedef unsigned long (*kallsyms_lookup_name_ptr)(const char *name);

static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"
};

kallsyms_lookup_name_ptr get_kallsyms_lookup_name(void) {
    //unsigned long *syscall_table;

    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    kallsyms_lookup_name_t kallsyms_lookup_name;
    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
    unregister_kprobe(&kp);

    //syscall_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");
    //return syscall_table;
    
    return kallsyms_lookup_name;
}

int init_module(void) {
    kallsyms_lookup_name_ptr kallsyms_lookup_name = get_kallsyms_lookup_name();
    printk("kallsyms_lookup_name is at address %lx\n", (unsigned long)kallsyms_lookup_name);
    return 0;
}

void cleanup_module(void) {
    printk("<1>Bye, Bye");
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("leo");
MODULE_DESCRIPTION("LKM get around unexported symbols");
