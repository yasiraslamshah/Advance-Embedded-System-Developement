#include <linux/kernel.h>
#include <linux/init.h>
//#include <linux/stat.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TIMER_MODULE_HW3");
MODULE_AUTHOR("Yasir Aslam Shah");

static struct timer_list my_timer;//  creating instance for struct timer_list

static char *username = "Yasir";
static int count;
static int wakeup_timer =500;//timer expiration in milliseconds

module_param(username,charp,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
module_param(wakeup_timer,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);

//callback function 
void callback_timer(unsigned long data)
{
	int ret_value;
	ret_value = mod_timer(&my_timer,jiffies + msecs_to_jiffies(wakeup_timer));
	if(ret_value)
	{
		printk("ERROR:Mod_timer Failed! ");
	}
	count = count+1;
	printk("Number of count :%d Fired",count);
}

//function to initialize and set up timer ,modifying timer,set expiration value
int init_mytimer(void)
{
	int ret_value;
	printk(KERN_INFO "TIMER MODULE\n");
	printk(KERN_INFO "%s",username);
	init_timer(&my_timer);
	setup_timer(&my_timer,callback_timer,0);

	printk("Firing Timer in %d ms",wakeup_timer);
	ret_value = mod_timer(&my_timer,jiffies+msecs_to_jiffies(wakeup_timer));
	if(ret_value)
	{
		printk("ERROR:Mod_Timer Failed!");
	
	}
	return 0;
}

//kernel module function to exit
void cleanup_timer(void)
{
	int ret_value;
	ret_value = del_timer(&my_timer);
	if(ret_value)
	{
		printk("Timer: ACTIVE");
	}
	printk("DELETING Timer");
	return;
}
module_init(init_mytimer);
module_exit(cleanup_timer);	
