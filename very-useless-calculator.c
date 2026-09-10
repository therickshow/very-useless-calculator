#include <stdio.h>

int main(void)
{

    long long microseconds;
    long long months;
    long long weeks;
    long long days;
    long long hours;
    long long minutes;
    long long seconds;

    const long long microseconds_per_month = 30LL * 24 * 60 * 60 * 1000000;
    const long long microseconds_per_week = 7LL * 24 * 60 * 60 * 1000000;
    const long long microseconds_per_day = 24LL * 60 * 60 * 1000000;
    const long long microseconds_per_hour = 60LL * 60 * 1000000;
    const long long microseconds_per_minute = 60LL * 1000000;
    const long long microseconds_per_second = 1000000;



 
    printf("Enter a time in microseconds: ");
    scanf("%lld", &microseconds);

   

    months = microseconds / microseconds_per_month;
    microseconds %= microseconds_per_month;

    weeks = microseconds / microseconds_per_week;
    microseconds %= microseconds_per_week;

    days = microseconds / microseconds_per_day;
    microseconds %= microseconds_per_day;

    hours = microseconds / microseconds_per_hour;
    microseconds %= microseconds_per_hour;

    minutes = microseconds / microseconds_per_minute;
    microseconds %= microseconds_per_minute;

    seconds = microseconds / microseconds_per_second;

 

    printf("%lld months, %lld weeks, %lld days, %lld hours, %lld minutes, %lld seconds\n",
           months, weeks, days, hours, minutes, seconds);

   
    getchar();
    getchar();

    return 0;
}
