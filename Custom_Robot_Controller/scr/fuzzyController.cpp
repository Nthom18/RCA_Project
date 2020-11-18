#include "fl/Headers.h"

void fuzzyController(fl::Engine* engine, fl::InputVariable* obstacle, fl::OutputVariable* mSteer)
{
  //Setup Engine  
  engine->setName("obstacleAvoidance");
  engine->setDescription("");

  //Setup input variable
  obstacle->setName("obstacle");
  obstacle->setDescription("");
  obstacle->setEnabled(true);
  obstacle->setRange(0.000 , 1.000);
  obstacle->setLockValueInRange(false);
  obstacle->addTerm(new fl::Ramp("left", 0.000, 0.500));
  obstacle->addTerm(new fl::Ramp("right", 0.500, 1.000));
  engine->addInputVariable(obstacle);
  
  //Setup output variable
  mSteer->setName("mSteer");
  mSteer->setDescription("");
  mSteer->setEnabled(true);
  mSteer->setRange(0.000, 1.000);
  mSteer->setLockValueInRange(false);
  mSteer->setAggregation(new fl::Maximum);
  mSteer->setDefuzzifier(new fl::Centroid(100));
  mSteer->setDefaultValue(fl::nan);
  mSteer->setLockPreviousValue(false);
  mSteer->addTerm(new fl::Ramp("left", 0.000, 5.000));
  mSteer->addTerm(new fl::Ramp("right", 0.500, 1.000));
  engine->addOutputVariable(mSteer);

  // Setup ruleblock
  fl::RuleBlock* mamdani = new fl::RuleBlock;
  mamdani->setName("mamdani");
  mamdani->setDescription("");
  mamdani->setEnabled(true);
  mamdani->setConjunction(fl::null);
  mamdani->setDisjunction(fl::null);
  mamdani->setImplication(new fl::AlgebraicProduct);
  mamdani->setActivation(new fl::General);
  mamdani->addRule(fl::Rule::parse("if obstacle is left then mSteer is right", engine));
  mamdani->addRule(fl::Rule::parse("if obstacle is right then mSteer is left", engine));
  engine->addRuleBlock(mamdani);
}