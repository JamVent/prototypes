#ifndef __JAM_CTRL_MGR_BASE_H__
#define __JAM_CTRL_MGR_BASE_H__


#include <memory>
#include <ostream>
#include <JamCtrlData.h>
#include "JamHw.h"
/**
 * @brief JamventCtrlMgrBase
 * @details Jamvent abstract base class for the control manager
 * 
 * 
 * 
 */
class JamCtrlMgrBase {
public:	
	JamCtrlMgrBase() {};
	virtual ~JamCtrlMgrBase() { };

	/**
	 * @brief perform any onetime intiailiation needed/
	 * 
	 */
	virtual void init() = 0;
	/////////////////////////////////////
	// control interface callable from UI thread.  
	//   All calls are threadsafe...
	//   
	/**
	 * @brief runThread
	 * @details run the thread, thread runs forever.
	 * 
	 * runs until kill is called (the destructor also implies kill).
	 */
	virtual void runThread() = 0;
									
	/**
	 * @brief killThread -- kill the thread before exiting.
	 * @details kill the running thread.
	 */
	virtual void killThread() = 0;

	/**
	 * @brief Check if the thread is running.
	 * @details checks if the thread is still running
	 *          
	 * @return true if the thread is running, false if not.
	 */
	virtual bool isThreadRunning() = 0;

	/**
	 * @brief Set the Time Interval to use for sampling inputs./
	 * 
	 * @param dt -- time interval to use.
	 */
	virtual void setTimeInterval(double dt) = 0;

	/**
	 * @brief setDebugLevel
	 * @details set the debug level to use for debugging 
	 *          must have done the setLogStream below
	 * 
	 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
	 */
	virtual void setDebugLevel(unsigned level) = 0;
	/**
	 * @brief setLogStream 
	 * @details set a logStream device to help with debugging...
	 * 
	 * @param ostr ostream object to send log data to.
	 */
	virtual void setLogStream(std::ostream *ostr) = 0; // set a log os stream device.  some place to send log info.

	/**
	 * @brief manually set the valve state.
	 * @details manullly override the valve state for a given valve.
	 * 
	 * @param open [description]
	 */
	//Now comes from JamCtrMgrInterface.
	//virtual void setValveAopen(bool open) = 0;
	//virtual void setValveBopen(bool open) = 0;
	//virtual void setValveCopen(bool open) = 0;
	//virtual void setValveDopen(bool open) = 0;


	/**
	 * @brief get a copy of the controld data
	 * @details the contents of the control data in the system.
	 * @return [description]
	 */
    // Now comes from JamCtrMgrInterface.
	//virtual const JamCtrlData getCtrlData() const = 0;	// returns a copy of the control data.


protected:
	/**
	 * @brief step the model one time step.
	 *        there is one implementation for the 
	 *        simululator and one for real hardware layer...
	 * @param dt delta time since last time we were called.
	 */
	virtual void step(double dt) = 0;		// step the model
};

#endif
