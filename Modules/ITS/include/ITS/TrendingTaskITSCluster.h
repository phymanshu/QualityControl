
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file    TrendingTaskITSCluster.h
/// \author  Ivan Ravasenga on the structure from Piotr Konopka
///

#ifndef QUALITYCONTROL_TRENDINGTASKITSCLUSTER_H
#define QUALITYCONTROL_TRENDINGTASKITSCLUSTER_H

#include "ITS/TrendingTaskConfigITS.h"
#include "QualityControl/PostProcessingInterface.h"
#include "QualityControl/Reductor.h"

#include <TAxis.h>
#include <TColor.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TTree.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace o2::quality_control::repository
{
class DatabaseInterface;
}

namespace o2::quality_control::postprocessing
{

/// \brief  A post-processing task which trends values, stores them in a TTree
/// and produces plots.
///
/// A post-processing task which trends objects inside QC database (QCDB). It
/// extracts some values of one or multiple
/// objects using the Reductor classes, then stores them inside a TTree. One can
/// generate plots out the TTree - the
/// class exposes the TTree::Draw interface to the user. The TTree and plots are
/// stored in the QCDB. The class is
/// configured with configuration files, see Framework/postprocessing.json as an
/// example.
///
/// \author Ivan Ravasenga on the structure from Piotr Konopka
class TrendingTaskITSCluster : public PostProcessingInterface
{
 public:
  TrendingTaskITSCluster() = default;
  ~TrendingTaskITSCluster() override = default;

  void configure(const boost::property_tree::ptree& config) override;
  void initialize(Trigger, framework::ServiceRegistryRef) override;
  void update(Trigger, framework::ServiceRegistryRef) override;
  void finalize(Trigger, framework::ServiceRegistryRef) override;

 private:
  // other functions (mainly style)
  void SetLegendStyle(TLegend* leg);
  void SetGraphStyle(TGraph* g, int col, int mkr);
  void SetGraphNameAndAxes(TH1* g, std::string name, std::string title,
                           std::string xtitle, std::string ytitle, double ymin,
                           double ymax, std::vector<std::string> runlist);
  void PrepareLegend(TLegend* leg, int layer);

  struct MetaData {
    Int_t runNumber = 0;
  };

  void trendValues(const Trigger& t, repository::DatabaseInterface& qcdb);
  void storePlots(repository::DatabaseInterface& qcdb);
  void storeTrend(repository::DatabaseInterface& qcdb);

  TrendingTaskConfigITS mConfig;
  MetaData mMetaData;
  Int_t ntreeentries = 0;
  UInt_t mTime;
  std::vector<std::string> runlist;
  std::unique_ptr<TTree> mTrend;
  std::unordered_map<std::string, std::unique_ptr<Reductor>> mReductors;

  const int col[7] = { 1, 2, 3, 4, 5, 6, 7 };
  const int mkr[7] = { 8, 29, 34, 21, 22, 33, 23 };
  static constexpr int NLAYERS = 7;
  static constexpr int NTRENDSCLUSTER = 4;
  const int nStaves[NLAYERS] = { 12, 16, 20, 24, 30, 42, 48 };
  // const int nStaves[NLAYERS] = {48};
  const std::string trendtitles[NTRENDSCLUSTER] = { "Cluster Size",
                                                    "Stddev Cluster", "Cluster Number of Active chips", "Cluster Occupancy" };
  const std::string trendnames[NTRENDSCLUSTER] = { "mean", "rms", "activechips", "occupancy" };
  const std::string ytitles[NTRENDSCLUSTER] = {
    "Avg. Cluster Size (pixels)", "Stddev Cluster (/event/pixel)", "Cluster # Active chips", "Cluster Occupancy (/chip/event)"
  };
};

} // namespace o2::quality_control::postprocessing

#endif // QUALITYCONTROL_TRENDINGTASKITSCLUSTER_H
