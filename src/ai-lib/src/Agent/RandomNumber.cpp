#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    RandomNumber::RandomNumber()
    {
    }

    RandomNumber::RandomNumber(int iseed_in)
    {
      mbig  = 1000000000;
      mseed = 161803398;
      mz    = 0;
      fac   = 1./mbig;
      Reseed(iseed_in);
    }

    RandomNumber::RandomNumber(const RandomNumber &rhs)
    {
      *this = rhs;
    }

    void RandomNumber::Reseed(int iseed_in)
    {
      int mj = mseed - abs(iseed_in);
      int mk = 1;
      int i,j;
      int ii;
      mj %= mbig;
      ma[54] = mj;

      for(i = 1; i <= 54; i++)
        {
          ii = (21*i) % 55;
          ma[ii-1] = mk;
          mk = mj - mk;
          if(mk < mz)
            {
              mk = mk + mbig;
            }
          mj = ma[ii-1];
        }
      for(j = 0; j < 4; j++)
        {
          for(i = 1; i <= 55; i++)
            {
              ma[i-1] = ma[i-1] - ma[(i+30)%55];
              if(ma[i-1] < mz)
                {
                  ma[i-1] = ma[i-1] + mbig;
                }
            }
        }
      inext  = 0;
      inextp = 31;
    }

    double RandomNumber::Rand()
    {
      int mj;

      inext++;
      if(inext >= 56)
        {
          inext = 1;
        }
      inextp++;
      if(inextp >= 56)
        {
          inextp = 1;
        }
      mj = ma[inext-1] - ma[inextp-1];
      while(mj < mz)
        {
          mj = mj + mbig;
        }
      if(mj > mbig)
        {
          mj %= mbig;
        }
      ma[inext-1] = mj;
      return mj*fac;
    }

    RandomNumber &RandomNumber::operator=(const RandomNumber &rhs)
    {
      mbig  = rhs.mbig;
      mseed = rhs.mseed;
      mz    = rhs.mz;
      fac   = rhs.fac;
      inext = rhs.inext;
      inextp = rhs.inextp;
      for(int i = 0; i < 55; i++)
        {
          ma[i] = rhs.ma[i];
        }
      return *this;
    }

  }
}
