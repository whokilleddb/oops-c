# OOPs-C

Wikipedia describes `C` as: 

> C is an imperative procedural language

Aka, it is not _OOPS_. Therefore, it gets a lot of backlash from the _cool-oops-programmers_ who like to flex their classes-containg-both-data-and-function-together. So, this repository is an attempt to one-up those programmers and implement the OOPs-like-features in C!

## Motivation, aka, WHY WOULD YOU DO THAT? THIS IS LITERALLY WHAT C++ IS FOR
To that I say sir: 
- C++ SUCKS. IT IS HORRIBLE AND I DONT WANT TO SEE IT (even our Lord and Saviour Linus Trovalds said [this](https://harmful.cat-v.org/software/c++/linus))
- Because I can.


## Data 🤝 Functions

So, if we gotta make OOPs-like features in C, we gotta start thinking in _"Classes"_. Classes in OOPs languages contain data and functions and that's their major advantage. The closest thing to such an arrangement in C would be `Structs`. They already contain different kinds of data, so that's half the job done. Now comes the fun part: Function Pointers. 

For those of you unfamiliar with Function Pointers, they are essentially just pointers, which point to functions (duh). And since they are essentially pointers, we can store their value in a struct, which, in a way would allow the structs to store functions as well, a lot like classes.

Function pointers might look scary at first, but they are easy once you grasp a hold of them. For example, consider the following C function:
```c
int add(int a, int b) {
    return a+b;
}
```
A pointer to the function would be defined as: 

```c
int (*add_ptr)(int, int); // Declare a function pointer that points to a function taking two integers and returning an integer
add_ptr = &add; // Assign the address of the 'add' function to the function pointer
```

> Note that you can directly use `add_ptr = add`; as the function name decays to a pointer

Then you can use the function pointer in place of the function with:
```c
add_ptr(3, 5);
```

Neat, so now that we have a way of combining data and functions in a struct, let us see an example of this.

## Show and Tell
In this repository, we write a very simple program that takes in a domain name and resolves the corresponding IPv4 and IPv6 addresses.

The header file in `includes/domaininfo.h` contains the following struct and function :
```c
// Domain Struct
struct _DomainInfo {
    char domainname[255];
    char ipv4[INET_ADDRSTRLEN];
    char ipv6[INET6_ADDRSTRLEN];
    int (*resolve)(struct _DomainInfo *);
    void (*display)(struct _DomainInfo *);
};
typedef struct _DomainInfo DomainInfo;

// function prototypes
int resolve(DomainInfo * domaininfo);
void display(DomainInfo *domaininfo);
```

The struct contains the following "data" fields:
- `domainname`: The domain to resolve
- `ipv4`: The IPv4 address of the associated domain
- `ipv6`: The IPv6 address of the associated domain

It also contains pointers to the following functions (which we can consider analogous to the methods of a Class):
- `resolve`: A pointer to a function that takes a pointer to a `DomainInfo` struct and returns an `int`. This function would be responsible for resolving the domain name into IP addresses.
- `display`:  A pointer to a function that takes a pointer to a `DomainInfo` struct and returns nothing, aka, `void`. This function would print the contents of the struct. 

There is another function worth mentioning and that is the `init()` function which has the following definition:
```c
DomainInfo * init(char *domain);
```

This function is analogous to a constructor in OOPs classes as it allocates memory for the `DomainInfo` struct on the heap. initializes it with the domain name and copies over the address of the functions into their respective pointers. I will skip the internal workings of these functions but feel free to check out the code (don't be lazy, go ahead, it won't hurt you to read a little code. I have left comments along the way).

However, let's look at the `main()` function:
```c
int main(int argc, char * argv[]) {
    printf("[i] Resolve IPv4 from Domain Name in C - Oops style\n");
    
    // Check Command line arguments
    if (argc!=2) {
        fprintf(stderr, "[!] Usage: %s <domain name>\n", argv[0]);
        return -1;
    }

    // initialize new struct
    DomainInfo * d_info = init(argv[1]);
    if (d_info == NULL) {
        return -1;
    }
    // Resolve domain name
    int _res = d_info->resolve(d_info);
    // Check results
    if (_res != 0) {
        free(d_info);
        return -1;
    }
    // Display results
    d_info->display(d_info);
    // Free struct
    free(d_info);
    return 0;
}
```

First, we have our usual check for the CLI args where we check whether the right number of arguments are supplied or not. Then we use the `init()` function to _initialize_ our struct with the domain name and the relevant function addresses and return an _"instance"_ (which is just a pointer to the data on the heap) of the Struct(think: Class). 

Then, we call the `resolve()` method from the `DomainInfo` struct and pass it the current _instance_(again, we are just passing the pointer to the data to the heap). It is analogous to the `this`/`self` keyword associated with most languages. 

If the address resolution succeeds, we call the `display()` method from the `DomainInfo` struct and pass to it the current _instance_ to print out the respective values. 

Finally, when all is said and done, we can free the memory allocated on the heap. We could have included a function in the struct to do this as well (kinda like a destructor), but it has been left as an exercise for the readers, so go ahead and have some fun with that 😏

## Compiling and Running

The given example can be easily compiled with:
```bash
$ make
gcc -O2 -g -Wall -pipe -I includes -o resolver src/main.c
```

This will produce a binary called `resolver` in the project directory, which can be run as:

```bash
$ ./resolver www.example.com
[i] Resolve IPv4 from Domain Name in C - Oops style
[i] Domain Name: www.example.com
[i] Function resolve(): 0x55673f1fe3e0
[i] Function display(): 0x55673f1fe350
[i] IPv4 Address: 93.184.216.34
[i] IPv6 Address: 2606:2800:220:1:248:1893:25c8:1946
```