/* A function to read from a root file
   and create plots to visualise the data

   gary.smith@ed.ac.uk
   01 Feb 2021
   
   This program was written for the EdMedPhysics 
   projects in 2021 to assist with data analysis.
   
   Input:
   A root file which is the output from
   the Geant4 simulation.
   
   Output: 
   1) an output root file containing new histograms
   2) pdfs of the histograms
   
   How to run:
   
   From terminal command line
   $ root make_histos.C

   From the root prompt
   $ root
   [0] .x make_histos.C
   
*/
void make_histos(){
  
  // The name of the file to input, 
  // ie the simulation output file.
  char filename[128] = "EdMedPhysics.root";

  // Declare a TFile object to read in data from.
  // Use the filename string from above.
  TFile * input_file = TFile::Open(filename);
  
  // A TTree is an ntuple with variables 
  // stored with different values per entry.
  TTree * tree = (TTree *) input_file->Get("EdMedPh");

  // Declare variables to read from the tree.
  // The same variable names are used here as in the
  // input file but they could be anything.
  double Edep; // energy deposited during the hit
  double X, Y, Z; // coordinates of the hit
  
  // Connect these variables to the ones in the TTree:
  // &Edep e.g assigns the address of the variable above 
  // to the variable "Edep" in the tree. 
  tree->SetBranchAddress("Edep",&Edep); 
  tree->SetBranchAddress("X",&X); 
  tree->SetBranchAddress("Y",&Y);
  tree->SetBranchAddress("Z",&Z); 
  // NB positions were recorded in mm
  
  // There is one entry per hit and
  // typically many per beam particle.
  int entries = tree->GetEntries();
  
  // Declare a 2D histogram
  // This will be used to plot the hits in the X-Y plane
  TH2F *hXY =  new TH2F("hXY","; X (cm) ; Y (cm)",
			100,-15.0,15.0, 100,-15.0,15.0);
  

  // This will be used to plot the hits in the Z-R plane
  TH2F *hZR =  new TH2F("hZR","; Z (cm) ; R (cm)",
			100,0.0,15.0, 100,0.0,5.0);
  
  // Declare a 3D histogram
  // Given this is a 3D histogram the number of bins 
  // per axis has been reduced 
  TH3F *hZXY =  new TH3F("hZXY","; Z (cm) ; X (cm); Y (cm)",
			 32,0.,20.,32,-15.0,15.0, 32,-15.0,15.0);  

  // The following for loop is used to iterate though the hits.
  // This is where the histograms are filled and data
  // cuts can be applied. 
  for (int i = 0; i < entries; i++) {
    
    // Get data for next energy deposit
    tree->GetEntry(i); 
    
    // Fill histograms with positions in cm
    // weighted by energy depostied
    hXY->Fill(X/10.,Y/10.,Edep);
    
    hZR->Fill(Z/10,sqrt(X*X+Y*Y)/10,Edep);
    
    hZXY->Fill(Z/10.,X/10.,Y/10.,Edep);
  }
 
  // Create a canvas to draw on
  // and for saving pdfs
  TCanvas * canvas = new TCanvas();
  
  canvas->SetLogz();

  // Dont show the stats box 
  gStyle->SetOptStat(0);

  // For Draw Options see 5.8.2 at the link below 
  //https://root.cern.ch/root/htmldoc/guides/users-guide/Histograms.html#drawing-histograms

  // Draw as heat map ie with
  // counts in colors
  hXY->Draw("colz");
  canvas->SaveAs("hXY.pdf");

  hZR->Draw("colz");
  canvas->SaveAs("hZR.pdf");

  canvas->SetLogz(0);

  // NB The following option may take some time to draw 
  // and/or open as an image 
  hZXY->Draw("ISO");

  // alternative draw option - comment in if desired
  //hZXY->Draw("BOX");

  canvas->SaveAs("hZXY.pdf");
  
  // Create a new file to save the histograms in.
  TFile * output_file = new TFile("my_new_histos.root","RECREATE");
  output_file->cd();
  
  hXY->Write();
  hZR->Write();
  hZXY->Write();
  
  output_file->Close();

  input_file->Close();
 
  // quit root and return to the terminal command prompt
  gApplication->Terminate();

}
