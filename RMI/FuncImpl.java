import java.rmi.Remote;
import java.rmi.RemoteException;
import java.time.ZonedDateTime;
import java.rmi.server.UnicastRemoteObject;
import java.util.Arrays;

public class FuncImpl
   extends UnicastRemoteObject implements RemoteFunc{

   public FuncImpl() throws RemoteException{
   }

   @Override
   public String getMessage() throws RemoteException{
      return "Hi! from Server \n Today is "+ZonedDateTime.now();
   }

   @Override
   public int getFibonacci(int num) throws RemoteException{
      if(num==0 || num==1)
         return num;
      int a,b,c, count;
      a=0;
      b=1;
      c=a+b;
      count=2;
      while(count<=num){
         c=a+b;
         a=b;
         b=c;
         count++; 
      }
      return c;
   }

    @Override
    public int[] log(int[] a, int b) throws RemoteException{
        for (int i=0;i<a.length;i++)
            a[i]=(int) (Math.log(a[i])/Math.log(b));
        return a;
    }

    @Override
    public double[] power(double[] a, int b) throws RemoteException {
        for (int i=0;i<a.length;i++)
            a[i]=(double) Math.pow(a[i],b);
        return a;
    }

    @Override
    public double[] square(double[] a) throws RemoteException {
        for (int i=0;i<a.length;i++)
            a[i]=(double) Math.sqrt(a[i]);
        return a;
    }

    @Override
    public double average(double[] a) throws RemoteException {
        int result=0;
        for (int i=0;i<a.length;i++)
            result+=a[i];
        return result;
    }

    @Override
    public double larger(double[] a, double b) throws RemoteException {
        int result=0;
        for (int i=0;i<a.length;i++)
            if (a[i]>b)
                result+=a[i];
        return result;
    }

    @Override
    public double deviation(double[] a) throws RemoteException {
        int result=1;
        for (int i=0;i<a.length;i++)
            result+=a[i]*a[i];
        return result;
    }

}
