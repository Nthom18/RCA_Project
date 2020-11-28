#include "fl/Headers.h"

void fuzzyController(fl::Engine* engine, fl::InputVariable* obstacleLeft, 
  fl::InputVariable* obstacleCenter, fl::InputVariable* obstacleRight,
  fl::OutputVariable* mSpeed, fl::OutputVariable* mSteer, fl::RuleBlock* mamdani)
{
  //Setup Engine
  engine->setName("obstacleAvoidance");
  engine->setDescription("");
  
  //Setup input variable
  obstacleLeft->setName("obstacleLeft");
  obstacleLeft->setDescription("");
  obstacleLeft->setEnabled(true);
  obstacleLeft->setRange(0.000 , 1.000); 
  obstacleLeft->setLockValueInRange(false);
  obstacleLeft->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleLeft->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleLeft->addTerm(new fl::Triangle("visible", 0.500, 0.750, 1.000, 1.0));
  engine->addInputVariable(obstacleLeft);

  obstacleCenter->setName("obstacleCenter");
  obstacleCenter->setDescription("");
  obstacleCenter->setEnabled(true);
  obstacleCenter -> setRange(0.000 , 1.000);
  obstacleCenter->setLockValueInRange(false);
  obstacleCenter->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleCenter->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleCenter->addTerm(new fl::Triangle("visible", 0.500, 0.750, 1.000, 1.0));
  engine->addInputVariable(obstacleCenter);

  obstacleRight->setName("obstacleRight");
  obstacleRight->setDescription("");
  obstacleRight->setEnabled(true);
  obstacleRight->setRange(0.000 , 1.000);
  obstacleRight->setLockValueInRange(false);
  obstacleRight->addTerm(new fl::Ramp("veryClose", 0.500, 0.000, 1.0));
  obstacleRight->addTerm(new fl::Triangle("close", 0.250, 0.500, 0.750, 1.0));
  obstacleRight->addTerm(new fl::Triangle("visible", 0.500, 0.750, 1.000, 1.0));
  engine->addInputVariable(obstacleRight);
  
 
  //Setup output variable - speed
  mSpeed->setName("mSpeed");
  mSpeed->setDescription("");
  mSpeed->setEnabled(true);
  mSpeed->setRange(0.000, 1.000);
  mSpeed->setLockValueInRange(false);
  mSpeed->setAggregation(new fl::Maximum);
  mSpeed->setDefuzzifier(new fl::Centroid(100));
  mSpeed->setDefaultValue(1.000);
  mSpeed->setLockPreviousValue(false);
  mSpeed->addTerm(new fl::Ramp("break", 0.5, 0, 1.0));
  mSpeed->addTerm(new fl::Triangle("slow", 0.250, 0.500, 0.750, 1.0));
  mSpeed->addTerm(new fl::Ramp("maximum", 0.500, 1.000, 1.0));

  engine->addOutputVariable(mSpeed);

  //Setup output variable - direction
  mSteer->setName("mSteer");
  mSteer->setDescription("");
  mSteer->setEnabled(true);
  mSteer->setRange(0.000, 1.000);
  mSteer->setLockValueInRange(false);
  mSteer->setAggregation(new fl::NormalizedSum);
  mSteer->setDefuzzifier(new fl::Centroid(100));
  mSteer->setDefaultValue(0.5);
  mSteer->setLockPreviousValue(false);
  //left - maximum left = 0
  mSteer->addTerm(new fl::Ramp("hardLeft", 0.250, 0, 1.0));
  mSteer->addTerm(new fl::Triangle("left", 0.125, 0.250, 0.375, 1.0));
  mSteer->addTerm(new fl::Triangle("softLeft", 0.250, 0.375, 0.500, 1.0));
  //right - maximum right = 1
  mSteer->addTerm(new fl::Ramp("hardRight", 0.750, 1.000, 1.0));
  mSteer->addTerm(new fl::Triangle("right", 0.625, 0.750, 0.875, 1.0));
  mSteer->addTerm(new fl::Triangle("softRight", 0.500, 0.650, 0.750, 1.0));

  engine->addOutputVariable(mSteer);

  // Setup ruleblock
  mamdani->setName("mamdani");
  mamdani->setDescription("");
  mamdani->setEnabled(true);
  mamdani->setConjunction(fl::null);
  mamdani->setDisjunction(new fl::NormalizedSum);///new fl::WeightedSum); //new fl::Maximum
  mamdani->setImplication(new fl::BoundedDifference);//AlgebraicProduct);
  mamdani->setActivation(new fl::General);
  //Direction
  mamdani->addRule(fl::Rule::parse("if obstacleLeft is veryClose then mSteer is hardRight", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleLeft is close then mSteer is right", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleLeft is visible then mSteer is softRight", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is veryClose then mSteer is hardLeft", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is close then mSteer is left", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is visible then mSteer is softLeft", engine));
  //Speed
  mamdani->addRule(fl::Rule::parse("if obstacleRight is veryClose or obstacleLeft is veryClose or obstacleCenter is veryClose then mSpeed is break", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is close or obstacleLeft is close or obstacleCenter is close then mSpeed is slow", engine));
  mamdani->addRule(fl::Rule::parse("if obstacleRight is visible or obstacleLeft is visible or obstacleCenter is visible then mSpeed is maximum", engine));
  engine->addRuleBlock(mamdani);
}