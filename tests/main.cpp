#include <window.h>
#include <fractal.h>
//#include <iostream>

int main(){
  Fractal f = Fractal(5,5);

  //auto iter = f.calculate_fractal(std::complex<float>(-1,0));
  // int i =0 ;
  // for(auto &val : f.render_screen(1.0)){
  //   if(i%5 ==0) std::cout<<"\n";
  //   std::cout<< val << " ";
  //   ++i;
  // }

  Window app = Window(1920, 1080);
  app.run();

  //std::cout<< iter << "\n";
}
