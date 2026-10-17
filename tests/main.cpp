#include <fractal.h>
#include <iostream>

int main(){
  Fractal f = Fractal(5,5);

  //auto iter = f.calculate_fractal(std::complex<float>(-1,0));
  int i =0 ;
  for(auto &val : f.render_screen(1.0)){
    if(i%5 ==0) std::cout<<"\n";
    std::cout<< val << " ";
    ++i;
  }

  //std::cout<< iter << "\n";
}
