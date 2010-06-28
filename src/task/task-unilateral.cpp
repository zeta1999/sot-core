/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      TaskUnilateral.cpp
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

//#define VP_DEBUG
#define VP_DEBUG_MODE 15

/* SOT */
#include <sot-core/task-unilateral.h>
#include <sot-core/debug.h>

using namespace std;
using namespace sot;
using namespace dynamicgraph;


#include <sot-core/factory.h>
SOT_FACTORY_TASK_PLUGIN(TaskUnilateral,"TaskUnilateral");


/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */


TaskUnilateral::
TaskUnilateral( const std::string& n )
  :Task(n)
  ,featureList()
  ,positionSIN( NULL,"sotTaskUnilateral("+n+")::input(vector)::position" )
  ,referenceInfSIN( NULL,"sotTaskUnilateral("+n+")::input(vector)::referenceInf" )
  ,referenceSupSIN( NULL,"sotTaskUnilateral("+n+")::input(vector)::referenceSup" )
  ,dtSIN( NULL,"sotTaskUnilateral("+n+")::input(double)::dt" )
{
  taskSOUT.setFunction( boost::bind(&TaskUnilateral::computeTaskUnilateral,this,_1,_2) );
  taskSOUT.clearDependencies();
  taskSOUT.addDependency( referenceSupSIN );
  taskSOUT.addDependency( referenceInfSIN );
  taskSOUT.addDependency( dtSIN );
  taskSOUT.addDependency( positionSIN );

  signalRegistration( referenceSupSIN<<dtSIN<<referenceInfSIN<<positionSIN );
}


/* --- COMPUTATION ---------------------------------------------------------- */
/* --- COMPUTATION ---------------------------------------------------------- */
/* --- COMPUTATION ---------------------------------------------------------- */

sotVectorMultiBound& TaskUnilateral::
computeTaskUnilateral( sotVectorMultiBound& res,int time )
{
  sotDEBUG(45) << "# In " << getName() << " {" << endl;
  const ml::Vector & position = positionSIN(time);
  sotDEBUG(35) << "position = " << position << endl;
  const ml::Vector & refInf = referenceInfSIN(time);
  const ml::Vector & refSup = referenceSupSIN(time);
  const double & dt = dtSIN(time);
  res.resize(position.size());
  for( unsigned int i=0;i<res.size();++i )
    {
      MultiBound toto((refInf(i)-position(i))/dt,(refSup(i)-position(i))/dt);
      res[i] = toto;
    }

  sotDEBUG(15) << "taskU = "<< res << std::endl;
  sotDEBUG(45) << "# Out }" << endl;
  return res;
}

/* --- DISPLAY ------------------------------------------------------------ */
/* --- DISPLAY ------------------------------------------------------------ */
/* --- DISPLAY ------------------------------------------------------------ */

void TaskUnilateral::
display( std::ostream& os ) const
{
  os << "TaskUnilateral " << name << ": " << endl;
}


