#include "../Simple_util.h"
#include "Grid_i.h"

// This is the main driver program for the time and date server.
extern void doSocket(void);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
#if !defined (ACE_LACKS_NETWORKING)
	printf("\nThis will print if there is a network");
#else
	printf("\nThis will print if there is not a network");
#endif	
  //doSocket();
  Server<Grid_Factory_i> server;
	int count = 0;
  printf("\n*****************************************************************\n\n");
  ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("\nGrid server\n\n")));
  argc = 12;
  char* myargv[argc+1] = {"-ORBDottedDecimalAddresses", "1", 
  "-ORBEnforcePreferredInterfaces", "1", 
  "-ORBPreferredInterfaces", "*=192.168.1.10",
  "-ORBListenEndpoints","iiop://192.168.1.10:9999", 
  "-o", "/etc/service.ior", 
  "-ORBdebuglevel", "2", 
  0};

  try
    {
     
     // Initialize the ORB.

      if (server.init ("Grid",
                       argc,
                       myargv) == -1){
	      return 1;
      }
      else
      {
      printf("\nStarting Grid Server");
        server.run ();
      }
    }
  catch (const CORBA::UserException& userex)
    {
      ACE_UNUSED_ARG (userex);
      userex._tao_print_exception ("User Exception in main");
      return -1;
    }
  catch (const CORBA::SystemException& sysex)
    {
      //ACE_UNUSED_ARG (sysex);
	sysex._tao_print_system_exception();
      //sysex._tao_print_exception ("System Exception in main ");
      return -1;
    }
  catch (const ::CORBA::Exception &e)
    {
      e._tao_print_exception ("CORBA exception in main");
      return 1;
    }
  return 0;
}
