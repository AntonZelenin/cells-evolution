#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GENETICS_ENGINEER_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GENETICS_ENGINEER_H_

#include <vector>
#include <unordered_map>
#include "genes.h"
#include "CellsEvo/cell.h"

namespace cells_evo::genetics {
class GeneConfigCacher {
  std::unordered_map<GeneType, GeneConfig> gene_config_cache_;
 public:
  GeneConfig &GetGeneConfig(GeneType gene_type) {
    if (gene_config_cache_.find(gene_type) != gene_config_cache_.end())
      // todo does it return a copy?
      return gene_config_cache_.find(gene_type)->second;
    gene_config_cache_.insert({gene_type, BuildGeneConfig(gene_type)});
    return gene_config_cache_.find(gene_type)->second;
  }
};

class Engineer {
  const int mutation_chance_ = 20;
  // todo it should explicitly use cacher, is it ok?
  GeneConfigCacher gene_config_cacher_{};

  [[nodiscard]] inline bool ShouldMutate() const;
 public:
  Gene CreateGene(GeneType gene_type);
  Gene CopyGene(Gene const &gene);
  std::vector<Gene> CopyGenes(std::unordered_map<GeneType, Gene> const &gene);
  std::vector<Gene> GenerateBaseGenes(core::CellType const &cell_type);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GENETICS_ENGINEER_H_
