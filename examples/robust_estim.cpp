
#include "ibex.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

/*================== data =================*/
const int N=10;          // number of measurements
const double L=10;       // the target & the beacons are in the area [0,L]x[0,L]
double beacons[N][2];    // positions (x,y) of the N beacons
double dist[N];          // distance between the target and each beacon
double BEACON_ERROR=0.1; // the uncertainty on the beacon position
double DIST_ERROR=0.1;   // the uncertainty on the distance
/*=========================================*/

// init data (simulate measurements)
void init_data() {
	srand(1111);

	double x=((double) rand()/RAND_MAX)*L;
	double y=((double) rand()/RAND_MAX)*L;
	cout << endl;
	cout << "******* Target *******" << endl;
	cout << "   x=" << x << " y=" << y << endl << endl;

	rand(); // just to synchronize with the number of "rand" in robust_estim2

	cout << "******* beacons *******" << endl;
	for (int i=0; i<N; i++) {
		beacons[i][0]=((double) rand()/RAND_MAX)*L;
		beacons[i][1]=((double) rand()/RAND_MAX)*L;
		dist[i]=sqrt(pow(beacons[i][0]-x,2)+pow(beacons[i][1]-y,2));
		cout << "   x=" << beacons[i][0] << " y=" << beacons[i][1] << " dist=" << dist[i] << endl;
	}
	cout << endl;
}

using namespace ibex;

int main() {

	init_data();

	Variable x(2);
	Variable y(2);
	Function distance(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));

	Function *m_func[N];      // for each measurement, we have a dist function
	Array<Ctc> m_ctc(N);     // and a contractor w.r.t the constraint dist-d=0

	for (int i=0; i<N; i++) {
		Variable x(2);

		IntervalVector a(2); // the beacon position + uncertainty
		a[0]=beacons[i][0]+BEACON_ERROR*Interval(-1,1);
		a[1]=beacons[i][1]+BEACON_ERROR*Interval(-1,1);

		Interval d; // the distance + uncertainty
		/*if (i==0) {
		  d=dist[i]+Interval(1,2);
		} else*/
			d=dist[i]+DIST_ERROR*Interval(-1,1);

		m_func[i] = new Function(x,distance(x,a)-d);

		m_ctc.set_ref(i,*new CtcFwdBwd(*m_func[i]));
	}

	// the initial box [0,L]x[0,L]
	double _box[][2]={{0,L},{0,L}};
	IntervalVector box(2,_box);

	// The q-intersection of the N contractors
	CtcQInter q(m_ctc,9);

	// Fixpoint
	CtcFixPoint fix(q);

	cout << "before =" << box << endl;
	try {
		fix.contract(box);
	} catch (EmptyBoxException&) { }
	cout << "after =" << box << endl;

	// cleanup
	for (int i=0; i<N; i++) {
		delete &m_ctc[i];
	}

	return 0;
}
