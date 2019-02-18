#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/list.h>
#include<linux/slab.h>
#include<linux/timer.h>

#define arraysize 50
char *inputanimal="dog";
int inputcount=2;
struct animals
{
	char name[15];
	uint8_t count;
	struct list_head list;

};

int globalcounterA;
int globalcounterB;

struct animals A;
struct animals B;

char unsorted_animals[][1000]={
			"dog","duck","tiger","frog","snake","sheep","goat","cow","bull","cat",
			"dog","duck","tiger","frog","snake","sheep","goat","cow","bull","cat",
		        "dog","duck","tiger","frog","snake","sheep","goat","cow","bull","cat",
		        "dog","duck","tiger","frog","snake","sheep","goat","cow","bull","cat",
                        "dog","duck","tiger","frog","snake","sheep","goat","cow","bull","cat",
			};
char temp[10];

MODULE_DESCRIPTION("Kernel Function HW3");
MODULE_AUTHOR("YASIR ASLAM SHAH");
MODULE_LICENSE("GPL");

module_param(inputanimal,charp,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
module_param(inputcount,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);


void animalbycount(unsigned int inputcount)
{
	
}

void animalbyname(char *inputname)
{
	
}



void sortarray(char unsorted_animals[][1000],int size)
{
	int i,j;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(unsorted_animals[j][0] > unsorted_animals[j+1][0])
			{
				strcpy(temp,unsorted_animals[j]);
				strcpy(unsorted_animals[j],unsorted_animals[j+1]);
				strcpy(unsorted_animals[j+1],temp);
			}
		}
	}
	printk(KERN_INFO"\nSorted List with Repeatition\n");
	for(i=0;i<size;i++)
	{
		printk(KERN_INFO"%s\t",unsorted_animals[i]);
	
	}
}

void free_memory(int a)
{
	struct animals *temp;
	if(a==1)
	{
		printk("\nMemory Used: %d\nMemory Freed :%d\n",globalcounterA,globalcounterA);
		list_for_each_entry(temp,&A.list,list)
		{
			kfree(temp);
		}
	}
	else if(a==2)
	{
		printk("\nMemory Used: %d\nMemory Freed :%d\n",globalcounterB,globalcounterB);
		list_for_each_entry(temp,&B.list,list)
		{
			kfree(temp);
		}
	}
}

void setA(void)
{
	int flag=0;
	struct animals *new1,*temp1;
	INIT_LIST_HEAD(&A.list);
	int i=0;
	for(i=0;i<arraysize;i++)
	{
		list_for_each_entry(temp1,&A.list,list)
		{
			if(strcmp(temp1->name,unsorted_animals[i])==0)
			{
				temp1->count++;
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			new1 =kmalloc(sizeof(*new1),GFP_KERNEL);
			globalcounterA = globalcounterA+sizeof(*new1);
			strcpy(new1->name,unsorted_animals[i]);
			new1->count=1;
			INIT_LIST_HEAD(&new1->list);
			list_add_tail(&(new1->list),&(A.list));
		}
		flag=0;
	}
}
	
void setB(void)
{
	int flag=0;
	struct animals *new2,*temp2;
	INIT_LIST_HEAD(&B.list);
	int i=0;
	for(i=0;i<arraysize;i++)
	{
		list_for_each_entry(temp2,&B.list,list)
		{
			if(strcmp(temp2->name,unsorted_animals[i])==0)
			{
				temp2->count++;
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			new2 =kmalloc(sizeof(*new2),GFP_KERNEL);
			globalcounterB = globalcounterB+sizeof(*new2);
			strcpy(new2->name,unsorted_animals[i]);
			new2->count=1;
			INIT_LIST_HEAD(&new2->list);
			list_add_tail(&(new2->list),&(B.list));
		}
		flag=0;
		i++;
	}
       
       	printk(KERN_INFO"\n***********\n");
	printk(KERN_INFO"Filtering by Name as %s\n",inputanimal);
	struct animals *tempX;
	list_for_each_entry(tempX,&B.list,list)
	{
		if(strcmp(tempX->name,inputanimal)==0)
		{
		printk(KERN_INFO"The %s with Count %d\n",tempX->name,tempX->count);
		}
	}
        printk(KERN_INFO"\n***********\n");

	printk(KERN_INFO"\nFiltering with Count greater or equal than %d\n",inputcount);
	struct animals *tempY;
	list_for_each_entry(tempY,&B.list,list)
	{
		if((tempY->count)>= inputcount)
		{
			printk(KERN_INFO"The %s with Count %d\n",tempY->name,tempY->count);
		}
	}
}

static int __init animals_init(void)
{
	struct timespec start,end,difference;
	int i;
	getnstimeofday(&start);

	printk(KERN_INFO"*******UNSORTED********\n");
	for(i =0;i<arraysize;i++)
	{
		printk(KERN_INFO"%s\t",unsorted_animals[i]);
	}
	
	printk(KERN_INFO"********SORTED*******\n");
	sortarray(unsorted_animals,arraysize);

	printk(KERN_INFO"*******SET_A*********\n");
	setA();
	free_memory(1);

	printk(KERN_INFO"*******SET_B********\n");
	setB();
	free_memory(2);

	printk(KERN_INFO"*******PRINT_LIST********\n");
	
	struct animals *temp2;
	list_for_each_entry(temp2,&B.list,list)
	{
		printk(KERN_INFO"\nThe '%s' at Count[%d]\n",temp2->name,temp2->count);
	}

	printk(KERN_INFO"*******TIME********\n");
	getnstimeofday(&end);
	difference = timespec_sub(end,start);
	printk(KERN_INFO"\nTIME DIFFERENCE :%lu",difference.tv_nsec);
	
	return 0;
}

static void __exit animals_exit(void)
{
	struct timespec start,stop,differ;
	differ=timespec_sub(stop,start);
	printk(KERN_INFO"Time to Exit: %lu\n",differ.tv_nsec);
}
module_init(animals_init);
module_exit(animals_exit);


