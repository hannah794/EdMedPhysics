//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: EdMedPhRunAction.cc 100946 2016-11-03 11:28:08Z gcosmo $
//
/// \file EdMedPhRunAction.cc
/// \brief Implementation of the EdMedPhRunAction class

#include "EdMedPhRunAction.hh"
#include "EdMedPhAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EdMedPhRunAction::EdMedPhRunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in EdMedPhAnalysis.hh
  auto analysisManager = G4RootAnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  
  // Creating histograms
  //  analysisManager->CreateH1("Edep_vs_z","Edep vs z; z(mm)", 500, 0.,50*cm);
  analysisManager->CreateH1("Edep_vs_z","Edep vs z;Edep (MeV); z(mm)", 500, 0.,50*cm);
  // analysisManager->CreateH1("Edep_vs_z_10cm","Edep vs z;Edep (MeV); z(mm)", 100, 0.,10*cm);
  // analysisManager->CreateH1("Edep_vs_z_20cm","Edep vs z;Edep (MeV); z(mm)", 100, 0.,20*cm);
  //  analysisManager->CreateH1("Length","trackL in material; z(mm)", 500, 0., 0.5*m);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("EdMedPh", "Edep spacial distribution");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EdMedPhRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4RootAnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "EdMedPhysics";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EdMedPhRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4RootAnalysisManager::Instance();
  //if ( analysisManager->GetH1(1) ) {
  if ( analysisManager->GetH1(0) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " E absorbed z : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Length") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Length") << G4endl;
    
    //     G4cout << " Length of radiation interaction : mean = " 
    //       << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length") 
    //       << " rms = " 
    //       << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;

  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
