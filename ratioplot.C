/// \file
/// \ingroup tutorial_hist
/// Example displaying two histograms and their ratio. This macro does not use the
/// class TRatioPlot. For ROOT version >= 6.08 TRatioPlot should be used. See
/// the other ratio plots examples in this folder.
///
/// \macro_image
/// \macro_code
///
/// \author Olivier Couet

void ratioplot( ) {
   // Start from hist tutorial example

   gStyle->SetOptStat("emrou");
   gStyle->SetStatFormat("7.5g");
   
   TFile *f = new TFile("/home/graham/Resampler/histos_Ediff.root");
   
   TH1D *h1 = (TH1D*)f->Get("hX2data");
   TH1D *h2 = (TH1D*)f->Get("hX2mc");

   // Define the Canvas
   TCanvas *c = new TCanvas("c", "canvas", 800, 800);
   c->SetTicks(1,1);
   c->SetGrid();

   // Upper plot will be in pad1
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.5, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   h1->SetStats(0);          // No statistics on upper plot
   h1->SetMinimum(1250.0);
   h1->SetMaximum(2000.0);
   h1->Draw();               // Draw h1
   h2->Draw("same");         // Draw h2 on top of h1

   // Avoid the first label (0) to be clipped.
   TAxis *axis = h1->GetYaxis();
   axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");
   axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   axis->SetLabelSize(15);

   // lower plot will be in pad
   c->cd();          // Go back to the main canvas before defining pad2
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.5);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridx(); // vertical grid
   pad2->SetGridy(); // horizontal grid   
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   // Define the ratio plot
   TH1D *h3 = (TH1D*)h1->Clone("h3");
   h3->SetLineColor(kBlack);
   h3->SetMinimum(0.8);  // Define Y ..
   h3->SetMaximum(1.35); // .. range
   h3->Sumw2();
//   h3->SetStats(0);      // No statistics on lower plot
   h3->Divide(h2);
   h3->SetMarkerStyle(21);
   h3->Draw("ep");       // Draw the ratio plot

   // h1 settings
   h1->SetLineColor(kBlue+1);
   h1->SetLineWidth(2);

   // Y axis h1 plot settings
   h1->GetYaxis()->SetTitleSize(20);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.55);

   // h2 settings
   h2->SetLineColor(kRed);
   h2->SetLineWidth(2);

   // Ratio plot (h3) settings
   h3->SetTitle(""); // Remove the ratio title

   // Y axis ratio plot settings
   h3->GetYaxis()->SetTitle("ratio h1/h2 ");
   h3->GetYaxis()->SetNdivisions(505);
   h3->GetYaxis()->SetTitleSize(20);
   h3->GetYaxis()->SetTitleFont(43);
   h3->GetYaxis()->SetTitleOffset(1.55);
   h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetYaxis()->SetLabelSize(15);

   // X axis ratio plot settings
   h3->GetXaxis()->SetTitleSize(20);
   h3->GetXaxis()->SetTitleFont(43);
   h3->GetXaxis()->SetTitleOffset(4.);
   h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetXaxis()->SetLabelSize(15);
}
