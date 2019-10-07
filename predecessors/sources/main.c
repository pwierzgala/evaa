///-------------------------------------------------------------------------------------------------------
/// Reading the temperature from all tempertature sensors conected thru a 1-wire interface
/// On Raspberry PI 3

///-------------------------------------------------------------------------------------------------------
/// Includes
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

///-------------------------------------------------------------------------------------------------------
/// Macros definition

#define TempSensorTest

///-------------------------------------------------------------------------------------------------------
/// Gloabal variables definition

#ifdef TempSensorTest
    float tempTab[2] = {-1,-1}; /// the table that stores the measured temperatures
#endif // TempSensorTest

///-------------------------------------------------------------------------------------------------------
/// Functions definition

///-------------------------------------------------------------------------------------------------------
/// Function reads temperature from all tempertature sensors conected thru a 1-wire interface
/// Param: referece to the table of floats in which the measured results are stored
/// Return: State of temperature reading:
///     Successfully reading:       0;
///     Error in opening folder:    1;
///-------------------------------------------------------------------------------------------------------
int readTempSensor(float tTab[])
{
    DIR * directory;
    struct dirent * file;
    const char dirPath[] = "/sys/bus/w1/devices";
    char devicePath[256];
    char dataRead[256];
    char chTemp[6];
    float fTemp;
    int i = 0;
    int numOfRead = 0;
    int fd;

    directory = opendir(dirPath);

    if (NULL == directory) {
        perror ("Couldn't open directory...");
        return 1;
    }
    while ((file = readdir(directory)))
    {
        if ( (DT_LNK == file->d_type) && ( NULL != strstr(file->d_name, "28-")) )
        {
            snprintf(devicePath, 256, "%s/%s/w1_slave", dirPath, file->d_name);
            fd  = open(devicePath, O_RDONLY);
            if (-1 == fd)
            {
                perror ("Couldn't open file...");
                return 1;
            }
            while ((numOfRead = read(fd, dataRead, 256))>0)
            {
                strncpy(chTemp, strstr(dataRead, "t=") + 2, 5 );
                fTemp = strtof(chTemp, NULL)/1000.0;
                /// Below the diagnostic line
                //printf("Device [%d]: %3.3f C\n",i,fTemp);
            }
            tTab[i]=fTemp;
            close(fd);
            i++;
        }
    }
    closedir(directory);
    return 0;
}

///-------------------------------------------------------------------------------------------------------
/// Main code

int main (void)
{

//-------------------------------------------------------------------------------------------------------
/// Block of the code that tests the temperature reading from sensors connected thru a 1-wire interface
#ifdef TempSensorTest
    int status  = 0;
    while (1)
    {
        if (!(status = readTempSensor(tempTab)))
        {
            for (int k = 0;k<2;k++)
            {
                printf("Device [%d]: %3.3f\n",k,tempTab[k]);
            }
        }

    }
#endif // TempSensorTest
//-------------------------------------------------------------------------------------------------------
    return 0;
}

