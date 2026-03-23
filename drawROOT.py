import ROOT, sys, os
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)

ROOT.gStyle.SetPaintTextFormat("1.2g")
toted = 0
rf = ROOT.TFile(sys.argv[1])
c = ROOT.TCanvas("","",800,600)
c.SetLogz(1)
c.SetLogy(1)
c.SetLogx(1)
c.SetRightMargin(0.16)
c.SetLeftMargin(0.16)

hname = [x.GetName() for x in rf.GetListOfKeys()]
for h in hname:
    tmph = rf.Get(h)
    tmph = tmph.Project3D("yx")
    tmph.SetXTitle("E [MeV]")
    tmph.SetYTitle("#Theta [rad.]")
    tmph.GetZaxis().SetTitleOffset(1.2)
    tmph.Draw("colz")
    c.SaveAs("eBeam_"+h+".pdf")

    tmph = rf.Get(h)
    tmph = tmph.Project3D("zx")
    tmph.SetXTitle("E [MeV]")
    tmph.SetYTitle("Z [mm]")
    tmph.GetZaxis().SetTitleOffset(1.2)
    tmph.Draw("colz")
    c.SaveAs("eBeam_EZ"+h+".pdf")

    tmph = rf.Get(h)
    tmph = tmph.Project3D("x")
    tmph.SetXTitle("E [MeV]")
    tmph.SetLineWidth(2)
    tmph.Draw("hist")
    c.SaveAs("eBeam_1d_"+h+".pdf")

    tmph = rf.Get(h)
    tmph = tmph.Project3D("z")
    tmph.SetXTitle("Z [mm]")
    c.SetLogx(0)
    tmph.SetLineWidth(2)
    tmph.Draw("hist")
    c.SaveAs("eBeam_z"+h+".pdf")


