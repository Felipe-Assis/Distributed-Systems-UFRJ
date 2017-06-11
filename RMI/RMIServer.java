import java.net.*;
import java.rmi.*;

public class RMIServer{

   public static void main(String[] args){
      try {
      	System.out.println("Initializing server...");
        FuncImpl impl=new FuncImpl();
        Naming.rebind("RMIServer", impl);
      }catch(Exception e) {
        System.out.println("Exception: " + e);
      }
   }
}