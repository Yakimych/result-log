open Utils;
open StorageUtils;

// TODO: Implement a pretty dialog instead
[@bs.val] external alert: string => unit = "alert";

module AddMutation = [%relay.mutation
  {|
    mutation AddResultMutation($input: results_insert_input!) {
      insert_results_one(object: $input) {
        player1 {
          id
          name
        }
        player2 {
          id
          name
        }
        player2goals
        player1goals
        extratime
        date
        id
      }
    }
  |}
];

module AddResultFragment = [%relay.fragment
  {|
    fragment AddResultFragment_CommunitySettings on community_settings {
      include_extra_time
      score_type
      max_selectable_points
      allow_draws
    }
  |}
];

let updateResultList = (store: ReasonRelay.RecordSourceSelectorProxy.t) => {
  ReasonRelayUtils.(
    switch (
      resolveNestedRecord(
        ~rootRecord=
          store->ReasonRelay.RecordSourceSelectorProxy.getRootField(
            ~fieldName="insert_results_one",
          ),
        ~path=[],
      )
    ) {
    | Some(node) =>
      createAndAddEdgeToConnections(
        ~store,
        ~node,
        ~connections=[
          {
            parentID: ReasonRelay.storeRootId,
            key: "CommunityStartPage_query_results_connection",
            filters: None,
          },
        ],
        ~edgeName="resultsEdge",
        ~insertAt=End,
      )
    | None => ()
    }
  );
};

let tryGetPlayerFromMutationResult =
    (store: ReasonRelay.RecordSourceSelectorProxy.t, playerPath: string) =>
  ReasonRelayUtils.resolveNestedRecord(
    ~rootRecord=
      store->ReasonRelay.RecordSourceSelectorProxy.getRootField(
        ~fieldName="insert_results_one",
      ),
    ~path=[playerPath],
  );

let addPlayerToStore = (store: ReasonRelay.RecordSourceSelectorProxy.t, node) =>
  ReasonRelayUtils.createAndAddEdgeToConnections(
    ~store,
    ~node,
    ~connections=[
      {
        parentID: ReasonRelay.storeRootId,
        key: "CommunityStartPage_query_players_connection",
        filters: None,
      },
    ],
    ~edgeName="playersEdge",
    ~insertAt=End,
  );

let updatePlayerList =
    (store: ReasonRelay.RecordSourceSelectorProxy.t, playerPath: string) => {
  switch (tryGetPlayerFromMutationResult(store, playerPath)) {
  | Some(playerResult) => addPlayerToStore(store, playerResult)
  | None => ()
  };
};

[@react.component]
let make =
    (
      ~communitySettingsFragment,
      ~playerPickerFragment,
      ~communityName: string,
      ~onResultAdded,
    ) => {
  let communitySettingsFragment =
    AddResultFragment.use(communitySettingsFragment);
  let (mutate, isAddingResult) = AddMutation.use();

  let (getMostUsedPlayer, updateUsedPlayers) =
    useMostUsedPlayer(communityName);

  let (maybePlayer1Name, setMaybePlayer1Name) =
    React.useState(_ => getMostUsedPlayer());
  let (goals1, setGoals1) = React.useState(_ => 0);

  let (maybePlayer2Name, setMaybePlayer2Name) = React.useState(_ => None);
  let (goals2, setGoals2) = React.useState(_ => 0);

  let (extraTime, setExtraTime) = React.useState(_ => false);
  let toggleExtraTime = () => setExtraTime(oldExtraTime => !oldExtraTime);

  let (date, setDate) = React.useState(_ => Js.Date.make());

  let resetState = () => {
    setMaybePlayer1Name(_ => getMostUsedPlayer());
    setMaybePlayer2Name(_ => None);
    setGoals1(_ => 0);
    setGoals2(_ => 0);
    setExtraTime(_ => false);
    setDate(_ => Js.Date.make());
  };

  let addResult = allowDraws => {
    let validationResult =
      ResultValidation.canAddResult(
        allowDraws,
        maybePlayer1Name,
        maybePlayer2Name,
        goals1,
        goals2,
        extraTime,
      );

    switch (validationResult) {
    | Error(message) => alert(message)
    | Ok((player1Name, player2Name)) =>
      updateUsedPlayers(player1Name, player2Name);

      let communityInput: AddResultMutation_graphql.Types.communities_obj_rel_insert_input = {
        data: {
          community_settings: None,
          description: None,
          id: None,
          name: Some(communityName),
          players: None,
          results: None,
        },
        on_conflict:
          Some({
            constraint_: `communities_name_key,
            update_columns: [|`name|],
            where: None,
          }),
      };

      let playersOnConflictInput: AddResultMutation_graphql.Types.players_on_conflict = {
        constraint_: `players_name_communityId_key,
        update_columns: [|`name|],
        where: None,
      };

      // TODO: use makeVariables instead?
      mutate(
        ~updater=
          (store: ReasonRelay.RecordSourceSelectorProxy.t, _response) => {
            updateResultList(store);
            updatePlayerList(store, "player1");
            updatePlayerList(store, "player2");
          },
        ~onCompleted=
          (_, maybeMutationErrors) => {
            switch (maybeMutationErrors) {
            | Some(e) => Js.Console.error2("Error adding result", e)
            | None =>
              resetState();
              onResultAdded();
            }
          },
        ~variables={
          input: {
            community: Some(communityInput),
            player1:
              Some({
                data: {
                  community: Some(communityInput),
                  communityId: None,
                  id: None,
                  name: Some(player1Name),
                  resultsAsPlayer1: None,
                  resultsAsPlayer2: None,
                },
                on_conflict: Some(playersOnConflictInput),
              }),

            player2:
              Some({
                data: {
                  community: Some(communityInput),
                  communityId: None,
                  id: None,
                  name: Some(player2Name),
                  resultsAsPlayer1: None,
                  resultsAsPlayer2: None,
                },
                on_conflict: Some(playersOnConflictInput),
              }),

            comment: None,
            communityId: None,
            date: Some(date |> Js.Date.toISOString),
            extratime: Some(extraTime),
            id: None,
            player1Id: None,
            player1goals: Some(goals1),
            player2Id: None,
            player2goals: Some(goals2),
          },
        },
        (),
      )
      |> ignore;
    };
  };

  <MaterialUi.Paper
    elevation={`Int(6)}
    style={ReactDOMRe.Style.make(~padding="25px 10px 10px 10px", ())}>
    <div
      style={ReactDOMRe.Style.make(~display="flex", ~marginBottom="10px", ())}>
      <PlayerPicker
        disabled=isAddingResult
        placeholderText="Player1"
        playerPickerFragment
        selectedPlayerName=maybePlayer1Name
        onChange={v => setMaybePlayer1Name(_ => Some(v))}
      />
      <GoalsPicker
        disabled=isAddingResult
        selectedGoals=goals1
        onChange={v => setGoals1(_ => v)}
        scoreType={communitySettingsFragment.score_type}
        maxSelectablePoints={communitySettingsFragment.max_selectable_points}
      />
      <GoalsPicker
        disabled=isAddingResult
        selectedGoals=goals2
        onChange={v => setGoals2(_ => v)}
        scoreType={communitySettingsFragment.score_type}
        maxSelectablePoints={communitySettingsFragment.max_selectable_points}
      />
      <PlayerPicker
        disabled=isAddingResult
        placeholderText="Player2"
        playerPickerFragment
        selectedPlayerName=maybePlayer2Name
        onChange={v => setMaybePlayer2Name(_ => Some(v))}
      />
    </div>
    <div
      style={ReactDOMRe.Style.make(
        ~display="flex",
        ~justifyContent="space-between",
        (),
      )}>
      <MaterialUi.Button
        disabled=isAddingResult
        variant=`Contained
        color=`Primary
        onClick={_ => addResult(communitySettingsFragment.allow_draws)}>
        {text("Submit")}
      </MaterialUi.Button>
      {communitySettingsFragment.include_extra_time
         ? <MaterialUi.FormControlLabel
             control={
               <MaterialUi.Checkbox
                 disabled=isAddingResult
                 color=`Default
                 checked=extraTime
                 onChange={_ => toggleExtraTime()}
               />
             }
             label={text("Extra Time")}
           />
         : React.null}
      <MaterialUi.TextField
        disabled=isAddingResult
        type_="date"
        value={`String(formatDate(date))}
        onChange={e => {
          let date = Js.Date.fromString(ReactEvent.Form.target(e)##value);
          if (DateFns.isValid(date)) {
            setDate(_ => date);
          };
        }}
      />
    </div>
  </MaterialUi.Paper>;
};
