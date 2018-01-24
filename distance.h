class Distance 
{
  
  public:
    
    float min_dist, left_dist, right_dist;
    int clockwise;
    
    //sets minimum distance value
    void set_min (float);
    //sets side distance values
    void set_sides (float, float);
    //returns minimum distance
    float min_dist_out() {return min_dist;}
    
    int clockwise_out() {
      //if 
      if(left_dist>right_dist){
        clockwise = 1;
      }
      
      if(right_dist>left_dist){
        clockwise = 0;
      }
      
      return clockwise;
    }
};