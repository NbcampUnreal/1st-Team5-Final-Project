
#include "LevelDataAsset.h"

#if WITH_EDITOR
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "WorldPartition/WorldPartition.h"
#include "UObject/SavePackage.h"

void ULevelDataAsset::BuildAssetListFromLevel()
{
    if (!LevelToOpen.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelToOpen is not valid."));
        return;
    }

    // Load the world to access World Partition data
    LevelToOpen.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ULevelDataAsset::OnLevelLoaded));
}

void ULevelDataAsset::OnLevelLoaded(const FSoftObjectPath& LoadedPath, UObject* LoadedObject)
{
    if (!LoadedObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load the level from soft object path"))
        return;
    }

    UWorld* World = Cast<UWorld>(LoadedObject);
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to cast object to UWorld"))
        return;
    }
    
    UWorldPartition* WorldPartition = World->GetWorldPartition();
    if (!WorldPartition)
    {
        UE_LOG(LogTemp, Error, TEXT("World does not have World Partition enabled"));
        return;
    }
    
    TSet<TSoftObjectPtr<UStaticMesh>> FoundStaticMeshes;
    TSet<TSoftObjectPtr<USkeletalMesh>> FoundSkeletalMeshes;
    TSet<TSoftObjectPtr<UMaterialInterface>> FoundMaterials;
    TSet<TSoftObjectPtr<UTexture>> FoundTextures;
    TSet<TSoftObjectPtr<UNiagaraSystem>> FoundNiagaraSystems;

    UActorDescContainerInstance* Container = WorldPartition->GetActorDescContainerInstance();
    if (!Container)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get the actor desc instance"));
        return;
    }

    int32 index = 1;
    for (FActorDescInstanceList::TIterator<> It(Container); It; ++It)
    {
        const FWorldPartitionActorDescInstance* ActorDesc = *It;
        if (!ActorDesc)
        {
            UE_LOG(LogTemp, Warning, TEXT("ActorDesc is null"));
            continue;
        }

        if (ActorDesc->GetActorNativeClass()->IsChildOf<AActor>()) // For broader scope
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor #%d : %s"), index++, *ActorDesc->GetActorNameString());
            AActor* Actor = ActorDesc->GetActor();
            if (!Actor)
            {
                UE_LOG(LogTemp, Warning, TEXT("GetActor failed."));
                continue;
            }

            TArray<UActorComponent*> Components = Actor->GetComponents().Array();
            for (UActorComponent* Component : Components)
            {
                if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Component))
                {
                    if (UStaticMesh* Mesh = StaticMeshComp->GetStaticMesh())
                    {
                        FoundStaticMeshes.Add(Mesh);
                        // Add material refs
                        for (UMaterialInterface* Mat : StaticMeshComp->GetMaterials())
                        {
                            if (Mat) FoundMaterials.Add(Mat);
                        }
                    }
                }
                else if (USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(Component))
                {
                    if (USkeletalMesh* SkelMesh = SkeletalComp->GetSkeletalMeshAsset())
                    {
                        FoundSkeletalMeshes.Add(SkelMesh);
                        for (UMaterialInterface* Mat : SkeletalComp->GetMaterials())
                        {
                            if (Mat) FoundMaterials.Add(Mat);
                        }
                    }
                }
                else if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Component))
                {
                    if (UNiagaraSystem* NiagaraSystem = NiagaraComp->GetAsset())
                    {
                        FoundNiagaraSystems.Add(NiagaraSystem);
                    }
                }
            }
        }
    }

    // Assign to arrays
    StaticMeshes = FoundStaticMeshes.Array();
    SkinnedMeshes = FoundSkeletalMeshes.Array();
    Materials = FoundMaterials.Array();
    Textures = FoundTextures.Array();
    NiagaraSystems = FoundNiagaraSystems.Array();

    UE_LOG(LogTemp, Warning, TEXT("World Partition asset scan complete!"));
    UE_LOG(LogTemp, Warning, TEXT("StaticMeshes: %d, SkeletalMeshes: %d, Materials: %d, Textures: %d, NiagaraSystems: %d"),
        StaticMeshes.Num(), SkinnedMeshes.Num(), Materials.Num(), Textures.Num(), NiagaraSystems.Num());

    // Log some examples of what was found
    if (StaticMeshes.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Example StaticMesh: %s"), *StaticMeshes[0].ToString());
    }
    if (Materials.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Example Material: %s"), *Materials[0].ToString());
    }

    // Mark the asset as dirty so it saves the changes
    MarkPackageDirty();
    
    // Optional: Auto-save the asset
    UPackage* Package = GetPackage();
    if (Package)
    {
        FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());
        FSavePackageArgs SaveArgs;
        SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
        UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);
        UE_LOG(LogTemp, Log, TEXT("LevelDataAsset saved successfully"));
    }
}

#endif