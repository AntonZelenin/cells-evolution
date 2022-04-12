#include "CellsEvo//genetics/engineer.h"

namespace cells_evo::genetics {
Gene Engineer::CopyGene(const Gene &gene) {
  Gene new_gene = gene;
  if (ShouldMutate()) {
    auto gene_conf = gene_config_cacher_.GetGeneConfig(new_gene.type);
    new_gene.value *= cells_evo::core::GetRandomNormalFloats(1, 0.25, 1)[0];
    new_gene.value = std::min(new_gene.value, gene_conf.max);
    new_gene.value = std::max(new_gene.value, gene_conf.min);
  }
  return new_gene;
}

Gene Engineer::CreateGene(GeneType gene_type) {
  auto conf = gene_config_cacher_.GetGeneConfig(gene_type);
  float gene_value = cells_evo::core::GetRandomNormalFloats(conf.base_value, conf.deviation, 1)[0];
  return {
      gene_value,
      gene_type
  };
}

bool Engineer::ShouldMutate() const {
  return cells_evo::core::GetRandomInts(0, 100 / mutation_chance_, 1)[0] == 1;
}

std::vector<Gene> Engineer::GenerateBaseGenes(const core::CellType &cell_type) {
//  switch (cell_type) {
//    case core::K_HUNTER:
  return std::vector<Gene>{
      CreateGene(GeneType::ENERGY_TO_DIVIDE),
      CreateGene(GeneType::DIRECTION_CHANGE_FACTOR),
      CreateGene(GeneType::DIVISION_COOLDOWN),
      CreateGene(GeneType::SIZE),
      CreateGene(GeneType::SPEED)
  };
//    case core::K_NONHUNTER:break;
//  }
}

std::vector<Gene> Engineer::CopyGenes(std::unordered_map<GeneType, Gene> const &genes) {
  std::vector<Gene> new_genes;
  new_genes.reserve(genes.size());
  for (auto &[gene_type, gene] : genes) {
    new_genes.push_back(CopyGene(gene));
  }
  return new_genes;
}
}
