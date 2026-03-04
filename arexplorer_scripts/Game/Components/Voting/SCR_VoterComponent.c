 [ComponentEditorProps(category: "GameScripted/Voting", description: "")]
 class SCR_VoterComponentClass : ScriptComponentClass
 {
 }

 class SCR_VoterComponent : ScriptComponent
 {
     protected PlayerController m_PlayerController;

     //------------------------------------------------------------------------------------------------
     //--- Public, anywhere
  static SCR_VoterComponent GetInstance()
     {
         if (GetGame().GetPlayerController())
             return SCR_VoterComponent.Cast(GetGame().GetPlayerController().FindComponent(SCR_VoterComponent));
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  static void InstantVote()
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (!manager)
             return;

         SCR_VoterComponent voterComponent = GetInstance();
         if (!voterComponent)
             return;

         array<EVotingType> votingTypes = {};
         array<int> votingValues = {};

         int count = manager.GetAllVotingsWithValue(votingTypes, votingValues, false, true);

         for (int i = count - 1; i >= 0; i--)
         {
             if (manager.HasAbstainedLocally(votingTypes[i], votingValues[i]))
             {
                 votingTypes.Remove(i);
                 votingValues.Remove(i);
                 continue;
             }

             if (manager.IsLocalVote(votingTypes[i], votingValues[i]))
             {
                 votingTypes.Remove(i);
                 votingValues.Remove(i);
                 continue;
             }
         }

         if (votingTypes.Count() == 1)
             voterComponent.Vote(votingTypes[0], votingValues[0]);

     }

     //------------------------------------------------------------------------------------------------
  static void InstantRemoveAndAbstainVote()
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (!manager)
             return;

         SCR_VoterComponent voterComponent = GetInstance();
         if (!voterComponent)
             return;

         array<EVotingType> votingTypes = {};
         array<int> votingValues = {};

         int count = manager.GetAllVotingsWithValue(votingTypes, votingValues, false, true);

         for (int i = count - 1; i >= 0; i--)
         {
             if (manager.HasAbstainedLocally(votingTypes[i], votingValues[i]))
             {
                 votingTypes.Remove(i);
                 votingValues.Remove(i);
                 continue;
             }

             if (manager.IsLocalVote(votingTypes[i], votingValues[i]))
             {
                 votingTypes.Remove(i);
                 votingValues.Remove(i);
                 continue;
             }
         }

         if (votingTypes.Count() == 1)
             voterComponent.RemoveVote(votingTypes[0], votingValues[0]);
     }

     //------------------------------------------------------------------------------------------------
     //--- Public, owner
  void Vote(EVotingType type, int value)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (!manager)
             return;

         Rpc(VoteServer, type, value);
         manager.VoteLocal(type, value);
     }

     //------------------------------------------------------------------------------------------------
  void RemoveVote(EVotingType type, int value = SCR_VotingBase.DEFAULT_VALUE)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (!manager)
             return;

         //~ Abstain vote (Local only)
         AbstainVote(type, value);

         Rpc(RemoveVoteServer, type, value);
         manager.RemoveVoteLocal(type, value);
     }

     //------------------------------------------------------------------------------------------------
  void AbstainVote(EVotingType type, int value = SCR_VotingBase.DEFAULT_VALUE)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (!manager)
             return;

         //~ Vote is active so remove it
         if (manager.IsLocalVote(type, value))
         {
             Rpc(RemoveVoteServer, type, value);
             manager.RemoveVoteLocal(type, value);
         }

         manager.AbstainVoteLocally(type, value);
     }

     //------------------------------------------------------------------------------------------------
  bool DidVote(EVotingType type, int value = SCR_VotingBase.DEFAULT_VALUE)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (manager)
             return manager.IsLocalVote(type, value);
         else
             return false;
     }

     //------------------------------------------------------------------------------------------------
  bool HasAbstained(EVotingType type, int value = SCR_VotingBase.DEFAULT_VALUE)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (manager)
             return manager.HasAbstainedLocally(type, value);
         else
             return false;
     }

     //------------------------------------------------------------------------------------------------
     //--- Protected, server
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void VoteServer(EVotingType type, int value)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (manager)
             manager.Vote(m_PlayerController.GetPlayerId(), type, value);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void RemoveVoteServer(EVotingType type, int value)
     {
         SCR_VotingManagerComponent manager = SCR_VotingManagerComponent.GetInstance();
         if (manager)
             manager.RemoveVote(m_PlayerController.GetPlayerId(), type, value);
     }

     //------------------------------------------------------------------------------------------------
     //--- Overrides
     override void OnPostInit(IEntity owner)
     {
         m_PlayerController = PlayerController.Cast(owner);
         if (!m_PlayerController)
         {
             Debug.Error2("SCR_VoterComponent", "SCR_VoterComponent must be attached to PlayerController!");
             return;
         }
     }
 }