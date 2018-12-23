#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef class Point{
   int x,y;
     public:
       Point() = default;                  //explicitly defaulted constructor
       Point(x,y){
	 x=x;
	 y=y; }
       int X(){return x;}
       int Y(){return y;}
}Point;

typedef class Pixel{
  cv::Vec3b Color;              //BGR not RGB  //uchar color[3] = {0}; works just as well
    Point topLeft(0,0), bottomRight(0,0);
      public:                               //obeys the holy laws of three-five-zero 
        Pixel() = default;                  
        Pixel(cv::Vec3b color,
	      Point topleft,
	      Point bottomright){
	   Color = color;
           topLeft = topleft;             
	   bottomRight = bottomright; }
        int Print(const string& a,
	          const string& s = NULL){
           if(a == "topLeft"){
               if(s == "X") return topLeft.X();
               if(s == "Y") return topLeft.Y(); }
           if(a == "bottomRight"){
               if(s == "X") return bottomRight.X();
               if(s == "Y") return bottomRight.Y(); }
           if(a == "R") return (int)Color[2];
           if(a == "G") return (int)Color[1];
           if(a == "B") return (int)Color[0];
	}
}Pixel;


  std::vector<Pixel> tree;
  cv::Mat3b image, visu;

  int N_size = 0;
  int count = 0;

int main(){

  image = cv::imread("image.jpg", CV_LOAD_IMAGE_COLOR);

  if(!image.data )                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

  int x = image.size().width;       //image.rows works just as well
  int y = image.size().height;      //image.cols works just as well

  int L_size  = x * y;
  
  while(L_size >= 1){               //get to the bottom right block(last 4 pixels)
    N_size += L_size;
    L_size /= 4; }

  L_size = x * y;
  
  
  
  //copy image pixels into a tree vector in blocks 
  for(int i=x-1; i>=0; i=i-2){        //move a step horizontally from right(end) to left(beginning)
    for(int j=y-1; j>=0; j=j-2){      //fill out the image vertically in blocks of 4  from bottom to top 
      tree[N_size - 1 - 4 * count].push_back(Pixel(image(i,j),                    //pixel location(1,1) in a block 
					           Point(i,j),
						   Point(i,j)));
      tree[N_size - 2 - 4 * count].push_back(Pixel(image(i,j-1),                    //(1,0)
						   Point(i,j-1),
						   Point(i,j-1)));
      tree[N_size - 3 - 4 * count].push_back(Pixel(image(i-1,j),                   //(0,1)
						   Point(i-1,j),
						   Point(i-1,j)));
      tree[N_size - 4 - 4 * count].push_back(Pixel(image(i-1,j-1),                  //(0,0)
						   Point(i-1,j-1),
						   Point(i-1,j-1)));
      count += 1;
    }
  }
  
  // every 4 pixels constituting a block are now averaged into 1 pixel to be part of a bigger block comprising 3 more averages
  for(int i=N_size -1 - 4 * count; i>-1, i--){
    tree[i].push_back(Pixel([(tree[4 * i +1].Print("B") + tree[4 * i +2].Print("B") + tree[4 * i +3].Print("B") + tree[4 * i +4].Print("B"))/4,
			     (tree[4 * i +1].Print("G") + tree[4 * i +2].Print("G") + tree[4 * i +3].Print("G") + tree[4 * i +4].Print("G"))/4,
			     (tree[4 * i +1].Print("R") + tree[4 * i +2].Print("R") + tree[4 * i +3].Print("R") + tree[4 * i +4].Print("R"))/4],
			    Point(tree[4 * i +1].Print("topLeft","X"), tree[4 * i +1].Print("topLeft","Y")),
			    Point(tree[4 * i +1].Print("bottomRight","X"), tree[4 * i +1].Print("bottomRight","Y"))));
  }

  void disp(int level){
    int start = 0;
    for(int i=0; i<level; i++)
        start = 4 * start + 1;
    if(start > N_size) return 0;

    for(int i=start; i < start*4; i++){
	x1 = tree[i].Print("topLeft","X");
        y1 = tree[i].Print("topLeft","Y");
        x2 = tree[i].Print("bottomRight","X");
        y2 = tree[i].Print("bottomRight","Y");
        for(int x=x1; x<x2+1; x++)
	  for(int y=y1; y<y2+1; y++)
            visu[x,y]=[tree[i].print(,"B"), tree[i].print(,"G"), tree[i].print(,"R")];
    }
    cv::namedWindow( "Display window", WINDOW_AUTOSIZE );           // Create a window for display.
    cv::imshow( "Display window", visu );                           // Show our new image inside it.

    cv::waitKey(0);                                                 // Wait for a keystroke in the window
    return 0;
  }
}
	
      


      

  
  
  
 

  
  
  
    
  
  
  
