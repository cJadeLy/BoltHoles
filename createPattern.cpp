#include <iostream>
#include <tuple>
#include <functional>
#include <memory>
#include <cmath> // std::abs


class FlangeProperties
{
public:

    int32_t number_of_bolts = 0;
    int32_t number_of_sections = 0;
    double distance_traveled = 0;

};
// Here is when we reset.
double max(double d1, double d2)
{
     std::cout << "last bolt = " << std::abs(d1 - d2) << " std::abs(d2 - d1) = " << std::abs(d2 - d1) << "\n";
    // This ensures we are taking the shortest turn when begining a new pass
    return ((d1 - d2) <= 180 && (d1 - d2) >= 0) ? (d1 - d2) : (d2 - d1);
}
double max2(double d1, double d2)
{
    std::cout << "last bolt = " << d1 << " first bolt better be 0 = " << d2 << "\n";
    // This ensures we are taking the shortest turn when begining a new pass
    return std::abs(d1 - d2) < std::abs(d2 - d1) ? std::abs(d1 - d2) : std::abs(d2 - d1);
}
// Returns the number of units up and down from starting bolt
std::tuple<int32_t, int32_t> getRotationPattern(int32_t number_of_bolts)
{

    int32_t up = number_of_bolts/2;
    int32_t down = up/2;
//    auto pattern = std::make_tuple(up, down);
//    std::cout << "The pattern will start at One and go up by ";
//    std::cout << std::get<0>(pattern);
//    std::cout << " and then go down by ";
//    std::cout << std::get<1>(pattern);

    return std::make_tuple(up, down);
}
// can print stuff of interest ..only used for debugging
void printStuff(std::shared_ptr<FlangeProperties> debug)
{
    std::cout<<debug->number_of_bolts<< " ^ num of bolts \n num of sections: " << debug->number_of_sections << "\n";
}

int main(void)
{
    std::shared_ptr<FlangeProperties> flange_properties = std::make_shared<FlangeProperties>();
    do{
        std::cout << "Please enter a valid number of bolts between 4 and 32 \n" << "To be valid, the number must also be a multiple of 4 \n";
        std::cin >> flange_properties->number_of_bolts;
    }
    while(flange_properties->number_of_bolts < 4 || flange_properties->number_of_bolts > 32);
    int32_t bolt_order[flange_properties->number_of_bolts];
    // Stores the location in degrees of the bolt we are tightening
    double bolt_location_in_degrees[flange_properties->number_of_bolts];
    // used to create start locations
    flange_properties->number_of_sections = flange_properties->number_of_bolts/4;
    int32_t start_locations[flange_properties->number_of_sections];
    // distance between each bolt
    double degrees = 360/static_cast<double>(flange_properties->number_of_bolts);
    
    // I had to hard code the reset locations.. still would like to see a pattern
    switch(flange_properties->number_of_sections)
    {
        case 1:
        {
            start_locations[0] = 1;
            break;
            
        }
        case 2:
        {
            start_locations[0] = 1;
            start_locations[1] = 2;
            break;
            
        }
        case 3:
        {
            start_locations[0] = 1;
            start_locations[1] = 2;
            start_locations[2] = 3;
            break;
        }
            
        case 4:
        {
            start_locations[0] = 1;
            start_locations[1] = 3;
            start_locations[2] = 2;
            start_locations[3] = 4;
            break;
        }
        case 5:
        {
            start_locations[0] = 1;
            start_locations[1] = 3;
            start_locations[2] = 5;
            start_locations[3] = 2;
            start_locations[4] = 4;
            break;
        }
        case 6:
        {
            start_locations[0] = 1;
            start_locations[1] = 4;
            start_locations[2] = 2;
            start_locations[3] = 5;
            start_locations[4] = 6;
            start_locations[5] = 3;
            break;
        }
        case 7:
        {
            start_locations[0] = 1;
            start_locations[1] = 4;
            start_locations[2] = 6;
            start_locations[3] = 2;
            start_locations[4] = 5;
            start_locations[5] = 3;
            start_locations[6] = 7;
            break;
            
        }
        case 8:
        {
            start_locations[0] = 1;
            start_locations[1] = 5;
            start_locations[2] = 3;
            start_locations[3] = 7;
            start_locations[4] = 2;
            start_locations[5] = 6;
            start_locations[6] = 4;
            start_locations[7] = 8;
            break;
        }
            
    }

    
    
    // unpack tuple
    int32_t up, down;
    std::tie(up, down) = getRotationPattern(flange_properties->number_of_bolts);
    bolt_location_in_degrees[0] = 0;
    bolt_order[0] = 1;
   
    int32_t k = 1;
    double temp;
    for(int32_t i = 1; i < flange_properties->number_of_bolts; i++)
    {

        if(i % 2 == 0 && i % 4 != 0)
        {
        bolt_order[i] = bolt_order[i - 1] - down;
        bolt_location_in_degrees[i] = static_cast<double>(degrees * (bolt_order[i]-1));
        // we will travel 90 degrees from where we currently are
        flange_properties->distance_traveled += 90;
            
        }
        else if(i%2 == 1 && i % 4 != 0)
        {
            bolt_order[i] = bolt_order[i - 1] + up;

            bolt_location_in_degrees[i] = static_cast<double>(degrees * (bolt_order[i] - 1));
             // we will travel 180 degrees from where we currently are
            flange_properties->distance_traveled += 180;
            
        }
        else if(i%4 == 0) // then we need to reset
        {
        temp = max(degrees * static_cast<double>(bolt_order[i - 1] - 1), degrees * static_cast<double>(start_locations[k]));
            bolt_order[i] = start_locations[k];
            
            // We can go forward or backwards to get to the next bolt, so lets not travel longer than we have to
            flange_properties->distance_traveled += (temp <= 180? temp : (360 - temp));
            bolt_location_in_degrees[i] = degrees * static_cast<double>(bolt_order[i]);
            k += 1;
        }
        

    }
    std::cout<< "order bolts were tightened: \n";
    for(int32_t i = 0; i < flange_properties->number_of_bolts; i++)
    {
     
            std::cout<< bolt_order[i] <<  " , ";
        
        
    }
    std::cout<< "\nlocation of each bolt in degrees: \n";
    for(int32_t i = 0; i < flange_properties->number_of_bolts; i++)
    {
        
        std::cout<< bolt_location_in_degrees[i] <<  " , ";
        if(i == (flange_properties->number_of_bolts - 1))
        {
            temp = max2(static_cast<double>(bolt_location_in_degrees[i]), bolt_location_in_degrees[0]);
            
            
            // this will need to change because sometimes we do more than one pass. this assumes we do only 3 passes total.. 30%, 60%, 100%
            flange_properties->distance_traveled += (temp * 2);
            // We can go forward or backwards to get to the next bolt, so lets not travel longer than we have to
            //flange_properties->distance_traveled += (temp <= 180? temp : (360 - temp));
            //bolt_location_in_degrees[i] = degrees * static_cast<double>(bolt_order[i]);
            //std::cout<< "\n temp = " << temp << "\n";
        }
        
        
    }
    
       std::cout<< "\n" << flange_properties->distance_traveled << " is the total distance traveled  " <<" \n";
        
        
    
    
    return 0;
}
