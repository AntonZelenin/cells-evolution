#include "CellsEvo/genetics/engineer.h"

namespace cells_evo::genetics {
Gene Engineer::CopyGene(const Gene &gene) {
  Gene new_gene = gene;
  if (ShouldMutate()) {
    auto gene_conf = gene_config_cacher_.GetGeneConfig(new_gene.type);
    new_gene.value += random_engine_.GetRandomNormalFloats(0, gene_conf.deviation, 1)[0];
    new_gene.value = std::min(new_gene.value, gene_conf.max);
    new_gene.value = std::max(new_gene.value, gene_conf.min);
  }
  return new_gene;
}

Gene Engineer::CreateGene(GeneType gene_type) {
  auto gene_conf = gene_config_cacher_.GetGeneConfig(gene_type);
  float gene_value = random_engine_.GetRandomNormalFloats(gene_conf.base_value, gene_conf.deviation, 1)[0];
  gene_value = std::min(gene_value, gene_conf.max);
  gene_value = std::max(gene_value, gene_conf.min);
  return {
      gene_value,
      gene_type
  };
}

bool Engineer::ShouldMutate() {
  return random_engine_.GetRandomInts(0, 100 / mutation_chance_, 1)[0] == 1;
}

std::vector<Gene> Engineer::GenerateBaseGenes(const core::CellType &cell_type) {
  switch (cell_type) {
    case core::K_HUNTER:
      return std::vector<Gene>{
          CreateGene(GeneType::ENERGY_TO_DIVIDE),
          CreateGene(GeneType::DIRECTION_CHANGE_FACTOR),
          CreateGene(GeneType::DIVISION_COOLDOWN),
          CreateGene(GeneType::SIZE),
          CreateGene(GeneType::HUNTING_SPEED),
          CreateGene(GeneType::IDLE_SPEED),
      };
    case core::K_NONHUNTER:
      return std::vector<Gene>{
          CreateGene(GeneType::ENERGY_TO_DIVIDE),
          CreateGene(GeneType::DIRECTION_CHANGE_FACTOR),
          CreateGene(GeneType::DIVISION_COOLDOWN),
          CreateGene(GeneType::SIZE),
          CreateGene(GeneType::HUNTING_SPEED),
          CreateGene(GeneType::IDLE_SPEED),
          CreateGene(GeneType::HARD_SHELL),
          CreateGene(GeneType::IGNORE_FOOD_NEAR_HUNTER),
          CreateGene(GeneType::IGNORE_HUNTER_NEAR_FOOD),
          CreateGene(GeneType::IGNORE_HUNTER_IS_RECESSIVE),
      };
  }
}

std::vector<Gene> Engineer::CopyGenes(std::unordered_map<GeneType, Gene> const &genes) {
  std::vector<Gene> new_genes;
  new_genes.reserve(genes.size());
  for (auto &[gene_type, gene] : genes) {
    new_genes.push_back(CopyGene(gene));
  }
  return new_genes;
}

GeneConfig &GeneConfigCacher::GetGeneConfig(GeneType gene_type) {
  if (gene_config_cache_.find(gene_type) != gene_config_cache_.end())
    // todo does it return a copy?
    return gene_config_cache_.find(gene_type)->second;
  gene_config_cache_.insert({gene_type, BuildGeneConfig(gene_type)});
  return gene_config_cache_.find(gene_type)->second;
}
}
