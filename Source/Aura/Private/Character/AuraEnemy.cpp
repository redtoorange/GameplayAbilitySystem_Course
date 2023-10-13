// Copyright Andrew McGuiness 2023


#include "Character/AuraEnemy.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCustomDepthStencilValue(250);
	Weapon->SetCustomDepthStencilValue(250);

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void AAuraEnemy::HighLightActor()
{
	bHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
	
	const FString s = FString::Printf(TEXT("Highlighting %s"), *GetName());
	GEngine->AddOnScreenDebugMessage(900, 5, FColor::Yellow, s);
}

void AAuraEnemy::UnHighlightActor()
{
	bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	
	const FString s = FString::Printf(TEXT("Stopped Highlighting %s"), *GetName());
	GEngine->AddOnScreenDebugMessage(900, 5, FColor::Red, s);
}
