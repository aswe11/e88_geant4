//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama

//E16G4_RunAction.hh 150105
//    Last modified at <2015-01-05 14:09:17 >
//

#ifndef E16G4_RUN_ACTION_H
#define E16G4_RUN_ACTION_H

#include <G4UserRunAction.hh>

class G4Run;
class E16G4_Analysis;
class E16G4_PrimaryGeneratorAction;

class E16G4_RunAction : public G4UserRunAction{
   private:
      E16G4_Analysis* anaMan;
      E16G4_PrimaryGeneratorAction *PrimaryGeneratorAction;
   
   public:
      E16G4_RunAction(E16G4_Analysis *anaManager=0,
            E16G4_PrimaryGeneratorAction *primarygeneratoraction=0);
      virtual ~E16G4_RunAction();

      virtual void BeginOfRunAction(const G4Run* aRun);
      virtual void EndOfRunAction(const G4Run* aRun);
      /*void SetProjectile( G4String &str ) { projectile_ = str; }
      void SetTarget( G4String &str ) { target_ = str; }
      void SetCwin( G4String &str ) { cwin_ = str; }
      void SetFrame( G4String &str ) { frame_ = str; }
      const G4String &GetProjectile() const { return projectile_;}
      const G4String &GetTarget() const { return target_;}
      const G4String &GetCwin() const { return cwin_;}
      const G4String &GetFrame() const { return frame_;}
*/
};

#endif//E16G4_RUN_ACTION_H


