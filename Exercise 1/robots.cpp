#include <iostream>
#include <iomanip>
#include <queue>
#include <string.h>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <windows.h>

using namespace std;


int n; // horizontal size of the map
int m; // vertical size size of the map

const int dir=4; // number of possible directions to go at any position
static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};

class node
{
    // current position
    int xPos;
    int yPos;
    // total distance already travelled to reach the node
    int level;
    // priority=remaining distance estimate
    int priority;  // smaller: higher priority

    public:
        node(int xp, int yp, int d, int p)
            {xPos=xp; yPos=yp; level=d; priority=p;}

        int getxPos() const {return xPos;}
        int getyPos() const {return yPos;}
        int getLevel() const {return level;}
        int getPriority() const {return priority;}

        void updatePriority(const int & xDest, const int & yDest)
        {
             priority=level+estimate(xDest, yDest)*10; //A*
        }

        // give better priority to going strait instead of diagonally
        void nextLevel() // i: direction
        {
             level+=1;
        }

        // Estimation function for the remaining distance to the goal.
        const int & estimate(const int & xDest, const int & yDest) const
        {
            static int xd, yd, d;
            xd=xDest-xPos;
            yd=yDest-yPos;

            // Admissible heuristic

            // Manhattan distance
            d=abs(xd)+abs(yd);

            // Inadmissible heuristic

            // d=xd*xd+yd*yd;

            return(d);
        }
};

// Determine priority (in the priority queue)
bool operator<(const node & a, const node & b)
{
  return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
string pathFind( const int & xStart, const int & yStart,
                 const int & xFinish, const int & yFinish,
                 int n, int m, int* closed_nodes_map,
                 int* open_nodes_map, int* dir_map, int* map)
{
    static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi=0;

    // reset the node maps
    for(y=0;y<m;y++)
    {
        for(x=0;x<n;x++)
        {
            closed_nodes_map[x*m+y]=0;
            open_nodes_map[x*m+y]=0;
        }
    }

    // create the start node and push into list of open nodes
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[x*m+y]=n0->getPriority(); // mark it on the open nodes map

    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x*m+y]=0;
        // mark it on the closed nodes map
        closed_nodes_map[x*m+y]=1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            string path="";
            while(!(x==xStart && y==yStart))
            {
                j=dir_map[x*m+y];
                c='0'+(j+dir/2)%dir;
                path=c+path;
                x+=dx[j];
                y+=dy[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {
            xdx=x+dx[i]; ydy=y+dy[i];

            if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || map[xdx*m+ydy]==1
                || closed_nodes_map[xdx*m+ydy]==1))
            {
                // generate a child node
                m0=new node( xdx, ydy, n0->getLevel(),
                             n0->getPriority());
                m0->nextLevel();
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx*m+ydy]==0)
                {
                    open_nodes_map[xdx*m+ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx*m+ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx*m+ydy]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[xdx*m+ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx*m+ydy]=(i+dir/2)%dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx &&
                           pq[pqi].top().getyPos()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    return ""; // no route found
}

void robot2(int tomX,int tomY, int &jerryX,int &jerryY, int *map){

	if ((jerryX>0)&&(tomX-jerryX>=0)&&(map[(jerryX-1)*m+jerryY]!=1)) jerryX--;	//jerry goes down
	else if ((jerryX<n-1)&&(tomX-jerryX<=0)&&(map[(jerryX+1)*m+jerryY]!=1)) jerryX++;	//up
	else if ((jerryY>0)&&(tomY-jerryY>=0)&&(map[jerryX*m+jerryY-1]!=1)) jerryY--;		//left
	else if ((jerryY<m-1)&&(tomY-jerryY<=0)&&(map[jerryX*m+jerryY+1]!=1)) jerryY++;		//right
//	return;
}

void display_map(int *map){
	int x,y;
	// display the map with the route
	cout<<endl;
    for(x=0;x<n;x++) {
            for(y=0;y<m;y++)
                if(map[x*m+y]==0)
                    cout<<" ";
                else if(map[x*m+y]==1){
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                    cout<<"X"; //obstacle
                    }
                else if(map[x*m+y]==2){
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                    cout<<"T"; //start
                    }
                else if(map[x*m+y]==3){
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    cout<<"B"; //route
                    }
                else if(map[x*m+y]==4){
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    cout<<"J"; //finish
                    }
            cout<<endl;
        }
        cout<<endl;
}


int main()
{

FILE * in_file;
string input_filename;
int new_input=1;

while (new_input==1){

SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
cout<<"Give input file name (e.g. input.txt): ";
cin>>input_filename;
cout<<endl;
in_file=fopen(input_filename.c_str(),"r");
if (in_file==NULL) {
                   cout<<"ERROR INPUT FILE\n\n";
                   continue;
                   }
fscanf(in_file,"%d %d",&n,&m);
//scanf("%d %d",&n,&m);
int map[n*m];
int closed_nodes_map[n*m]; // map of closed (tried-out) nodes
int open_nodes_map[n*m]; // map of open (not-yet-tried) nodes
int dir_map[n*m]; // map of directions
int FirstDir;
string route;

    // select start and finish locations
    int x_Tom, y_Tom, x_Jerry, y_Jerry, j, i;
    fscanf(in_file,"%d %d", &x_Tom, &y_Tom);
    fscanf(in_file,"%d %d", &x_Jerry, &y_Jerry);
	char c;
	fscanf(in_file,"%c",&c); //read newline
	for (i=0;i<n;i++){
        for(j=0;j<m;j++){
		fscanf(in_file,"%c",&c);
		if ((c=='X') || (c=='x')) map[i*m+j]=1;
		else map[i*m+j]=0;
		}
		fscanf(in_file,"%c",&c); //read newline
    }
    fclose(in_file);
	map[x_Tom*m+y_Tom]=2;
	map[x_Jerry*m+y_Jerry]=4;
    cout<<"Map Size (X,Y): "<<n<<","<<m<<endl;
    cout<<"Tom starts at: "<<x_Tom<<","<<y_Tom<<endl;
    cout<<"Jerry starts at: "<<x_Jerry<<","<<y_Jerry<<endl;

    display_map(map);
    cin.get(); // wait for a (Enter) keypress

    // get the route
    while ((x_Tom!=x_Jerry)||(y_Tom!=y_Jerry)){
    	map[x_Tom*m+y_Tom]=0;
    	string route=pathFind(x_Tom, y_Tom, x_Jerry, y_Jerry, n, m, closed_nodes_map, open_nodes_map, dir_map, map);
    	if(route=="") break;
    	c=route.at(0);
    	FirstDir=atoi(&c);
    	x_Tom+=dx[FirstDir];
    	y_Tom+=dy[FirstDir];
    	map[x_Tom*m+y_Tom]=2;
    	if ((x_Tom==x_Jerry)&&(y_Tom==y_Jerry)) map[x_Tom*m+y_Tom]=3;
    	display_map(map);
    	cin.get(); // wait for a (Enter) keypress
    	if ((x_Tom==x_Jerry)&&(y_Tom==y_Jerry)) break;
    	map[x_Jerry*m+y_Jerry]=0;
    	robot2(x_Tom,y_Tom,x_Jerry,y_Jerry,map);
    	map[x_Jerry*m+y_Jerry]=4;
    	if ((x_Tom==x_Jerry)&&(y_Tom==y_Jerry)) map[x_Tom*m+y_Tom]=3;
    	display_map(map);
    	cin.get(); // wait for a (Enter) keypress
    	map[x_Tom*m+y_Tom]=0;
    	route=pathFind(x_Tom, y_Tom, x_Jerry, y_Jerry, n, m, closed_nodes_map, open_nodes_map, dir_map, map);
    	c=route.at(0);
    	FirstDir=atoi(&c);
    	x_Tom+=dx[FirstDir];
    	y_Tom+=dy[FirstDir];
    	map[x_Tom*m+y_Tom]=2;
    	if ((x_Tom==x_Jerry)&&(y_Tom==y_Jerry)) map[x_Tom*m+y_Tom]=3;
    	display_map(map);
		cin.get(); // wait for a (Enter) keypress
    }

   	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    if(map[x_Tom*m+y_Tom]==3) {
                              SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                              printf("Tom caught Jerry!\n");
                              }
    else if(route=="") cout<<"Jerry was in another room"<<endl;
    cin.get(); // wait for a (Enter) keypress

SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
cout<<"Press 1 for new input or 0 for exit: ";
cin>>new_input;
}
return(0);
}
