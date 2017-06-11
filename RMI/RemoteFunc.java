import java.rmi.*;

public interface RemoteFunc extends Remote{

   public String getMessage() throws RemoteException;
   public int getFibonacci(int num) throws RemoteException;
   public double[] log (double[] a, int b) throws RemoteException;
   public double[] power (double[] a, int b) throws RemoteException;
   public double[] square (double[] a) throws RemoteException;
   public double average (double[] a) throws RemoteException;
   public double larger (double[] a, double b) throws RemoteException;
   public double deviation(double[] a) throws RemoteException;
}
