// Simple code to read the force/torque date using comedi
// Based on the code of Paul Griffiths 
// compile with: gcc main.c atinano.c sleep_time.c keyboard_hit.c -o run -lcomedi -latift-0.1 -lm

#include <stdio.h>
#include <unistd.h>
#include "atinano.h"
#include "math.h"
#include "sleep_time.h"
#include "keyboard_hit.h"

// nano43
//#define Comedi_Dev "/dev/comedi0"
//#define Calibration "/home/mecha/Sensors/ATI/Calibration/FT9820.cal"

// mini40
#define Comedi_Dev "/dev/comedi0"
#define Calibration "/home/biopsy/Documents/ATI/Calibration/FT11269.cal"

int main( void )
{
	double ft[6];	/**< Array of force torque readings. */
	double fs[6]; // force at the sensor's frame
	double R[2][2] = {{0,0},{0,0}}; // rotation matrix
	static double theta = 0;//1.223; // rotation angle
	int ch = 0; // get key
	double mag = 0.0; // force magnitude

	init_atinano(Comedi_Dev, Calibration);	/**< Initialize force/torque sensor and nidaq. */
	init_keyboard(); // start reading keyboard	

	while(ch != 'q') {
		readFTSensor();
		getLastFTReading( ft );
		system("clear");
		R[0][0] = cos(theta); R[0][1] = -sin(theta); 
		R[1][0] = sin(theta); R[1][1] = cos(theta);		
		fs[0] = R[0][0]*ft[0] + R[0][1]*ft[1];
		fs[1] = R[1][0]*ft[0] + R[1][1]*ft[1];
		fs[2] = ft[2];
		mag = sqrt( fs[0]*fs[0] + fs[2]*fs[2] );

		if(kbhit())
			ch = readch();

		printf("\nFx = %lf, \nFy = %lf, \nFz = %lf, \nmag = %lf\n", fs[0], fs[1], fs[2], mag );
		if (ch == 's') {
			if (fabs(fs[1]) < 0.005){
				printf("theta: %lf\n", theta);
				ch = 'q';
			}
			theta -= 0.0005;
		}

	}

	EndSensorReading();
	close_keyboard();
	
	return 1;
}
