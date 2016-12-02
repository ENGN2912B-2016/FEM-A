#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <algorithm.h>

using namespace Eigen;
using namespace std;

void print_output(string filename,string solvername,vector<double>& grids,vector<double>& u_ini,MatrixXd& u,double& h_space,double& k_time,double& T_end);

void differences_1d(string outputfilename,string solvername,MatrixXd &u,vector<double> &u_ini,vector<double>  &grid,double& k,double& h,double& T){
  //output u - u_ini per point per time step for plotting to turn into a video to show if plot flattens over time
  //print out yes/no
  cout<<solvername<<endl;

  //use different solver
    if (solvername=="BE" ) {
        solver_back_euler_1D(u,u_ini,grid,k,h,T) ;
      }
    else if (solvername=="E" ){
        solver_euler_1D(u,u_ini,grid,k,h,T) ;
    }
    else if (solvername=="CN" ){
        solver_crank_nicolson_1D(u,u_ini,grid,k,h,T);
    }
    else if (solvername=="DF" ){
        solver_DuFort_Frankel_1D(u,u_ini,grid,k,h,T);
    }
    else{
      cout << "No solver for the input" << endl;
    }


  // calculate the step difference
  MatrixXd u_diff=u;

  for(int i=0;i<u.rows();i++){
    if(i==0){
      for(int j=0;j<u.cols();j++){
        u_diff(i,j)=u(i,j)-u_ini[j];
      }
    }
    else{
      for(int j=0;j<u.cols();j++){
          u_diff(i,j)=u(i,j)-u(i-1,j);
      }
    }
  }

  //print the difference matrix into file
  string caselabel;
  caselabel=solvername+"_difference";
  print_output( outputfilename, caselabel,grid,u_ini, u_diff, h, k, T);




}


// study the  stability for the four basic algorithm
void convergence_1d(string outputfilename,string solvername,string boundaryname,vector<double> &u_ini,vector<double>& grid,double& k,double& h,double& T,double delta=10e-7){

  int ntime;
  int ngrids;
  ngrids=grid.size();
  ntime=T/k;
  MatrixXd u_original_solution(ntime,ngrids);

  //use different solver for the original solutio
    if (solvername=="BE" ) {
        solver_back_euler_1D(u_original_solution,u_ini,grid,k,h,T) ;
      }
    else if (solvername=="E" ){
        solver_euler_1D(u_original_solution,u_ini,grid,k,h,T) ;
    }
    else if (solvername=="CN" ){
        solver_crank_nicolson_1D(u_original_solution,u_ini,grid,k,h,T);
    }
    else if (solvername=="DF" ){
        solver_DuFort_Frankel_1D(u_original_solution,u_ini,grid,k,h,T);
    }
    else{
      cout << "No solver for the input" << endl;
    }

// stability study, give a small disturbulation delta on initial condition

  MatrixXd u_delta_plus(ntime,ngrids),u_delta_minus(ntime,ngrids),u_real(ntime,ngrids);
  vector<double> u_ini_plus(ngrids,0),u_ini_minus(ngrids,0);

  //update the initial condtion with disturb
  if (boundaryname=="1d_1"){
    boundary_1d_1(u_ini_plus,grid,delta);
    boundary_1d_1(u_ini_minus,grid,-delta);
  }
  else{

  }


  // solver the new problem
  //use different solver for the original solutio
    if (solvername=="BE" ) {
        solver_back_euler_1D(u_delta_plus,u_ini_plus,grid,k,h,T) ;
        solver_back_euler_1D(u_delta_minus,u_ini_minus,grid,k,h,T) ;
        solver_back_euler_1D(u_real,u_ini,grid,k,h,T) ;
      }
    else if (solvername=="E" ){
        solver_euler_1D(u_delta_plus,u_ini_plus,grid,k,h,T) ;
        solver_euler_1D(u_delta_minus,u_ini_minus,grid,k,h,T) ;
        solver_euler_1D(u_real,u_ini,grid,k,h,T) ;
    }
    else if (solvername=="CN" ){
        solver_crank_nicolson_1D(u_delta_plus,u_ini_plus,grid,k,h,T);
        solver_crank_nicolson_1D(u_delta_minus,u_ini_minus,grid,k,h,T);
        solver_crank_nicolson_1D(u_real,u_ini,grid,k,h,T);
    }
    else if (solvername=="DF" ){
        solver_DuFort_Frankel_1D(u_delta_plus,u_ini_plus,grid,k,h,T);
        solver_DuFort_Frankel_1D(u_delta_minus,u_ini_minus,grid,k,h,T);
        solver_DuFort_Frankel_1D(u_real,u_ini,grid,k,h,T);
    }
    else{
      cout << "No solver for the input" << endl;
    }
  // save the data for different cases

  //print the result with ini+delta
  string caselabel;
  caselabel=solvername+"_plus";
  print_output( outputfilename, caselabel,grid,u_ini_plus, u_delta_plus, h, k, T);

  //print the result with ini-delta
  caselabel=solvername+"_minus";
  print_output( outputfilename,caselabel,grid, u_ini_minus, u_delta_minus, h,k,T);

  // print the result for the original solution
   caselabel=solvername+"_real";
  print_output(outputfilename, caselabel, grid,u_ini,u_real, h, k, T);

}
