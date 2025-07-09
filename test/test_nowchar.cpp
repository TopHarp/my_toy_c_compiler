
void fun(int val)
{    
    printf("\n -fun b:%d-\n", val);
    return;
}

int main()
{
    // int b;
    int i, b;
    b = 0xfe;
    printf("\n -hi %d-\n", b);
    fun();
    // if (b == 0xfe)
    // {
    //     printf("\n -if %d-\n", b);
    // }
    i = 0;
    printf("\n init i:%d b:%d \n", i, b);
    while (i < 10)
    {
        i++;
        b--;
        printf("\n -while i:%d b:%d -\n", i, b);   
    }
    printf("\n -while done %d-\n", b);   

    // return ;
}

