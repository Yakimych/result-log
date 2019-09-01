open Utils;
open Queries;
open Mutations;
open StorageUtils;

// TODO: Implement a pretty dialog instead
[@bs.val] external alert: string => unit = "alert";

[@react.component]
let make =
    (
      ~dateFrom: option(Js.Date.t)=?,
      ~dateTo: option(Js.Date.t)=?,
      ~communityName: string,
    ) => {
  let allPlayersQuery = AllPlayersQueryConfig.make(~communityName, ());
  let (playersQuery, _) =
    AllPlayersQuery.use(~variables=allPlayersQuery##variables, ());

  let (addResultMutation, _, _) = AddResultMutation.use();

  let (mostUsedPlayerRef, updateUsedPlayers) =
    useMostUsedPlayer(communityName);

  let (maybePlayer1Name, setMaybePlayer1Name) =
    React.useState(_ =>
      React.Ref.current(mostUsedPlayerRef) |> Js.Nullable.toOption
    );
  let (goals1, setGoals1) = React.useState(_ => 0);

  let (maybePlayer2Name, setMaybePlayer2Name) = React.useState(_ => None);
  let (goals2, setGoals2) = React.useState(_ => 0);

  let (extraTime, setExtraTime) = React.useState(_ => false);
  let toggleExtraTime = () => setExtraTime(oldExtraTime => !oldExtraTime);

  let (date, setDate) = React.useState(_ => Js.Date.make());
  let (isAddingResult, setIsAddingResult) = React.useState(_ => false);

  let resetState = () => {
    setMaybePlayer1Name(_ =>
      mostUsedPlayerRef |> React.Ref.current |> Js.Nullable.toOption
    );
    setMaybePlayer2Name(_ => None);
    setGoals1(_ => 0);
    setGoals2(_ => 0);
    setExtraTime(_ => false);
    setDate(_ => Js.Date.make());
  };

  let addResult = () =>
    switch (maybePlayer1Name, maybePlayer2Name, goals1, goals2, extraTime) {
    | (None | Some(""), _, _, _, _)
    | (_, None | Some(""), _, _, _) => alert("You must select both players!")
    | (_, _, goals1, goals2, _) when goals1 === goals2 =>
      alert("A game cannot end in a draw!")
    | (_, _, goals1, goals2, extraTime)
        when Js.Math.abs_int(goals1 - goals2) != 1 && extraTime =>
      alert(
        "Games with Extra Time cannot have more than one goal difference!",
      )
    | (Some(player1Name), Some(player2Name), _, _, _)
        when player1Name === player2Name =>
      alert("You must select two DIFFERENT players!")
    | (Some(player1Name), Some(player2Name), goals1, goals2, extraTime) =>
      setIsAddingResult(_ => true);
      updateUsedPlayers(player1Name, player2Name);

      addResultMutation(
        ~variables=
          AddResultMutationConfig.make(
            ~communityName,
            ~player1Name,
            ~player2Name,
            ~date=date->withCurrentTime(Js.Date.make())->toJsonDate,
            ~player1Goals=goals1,
            ~player2Goals=goals2,
            ~extraTime,
            (),
          )##variables,
        ~refetchQueries=
          _ =>
            [|
              ReasonApolloHooks.Utils.toQueryObj(
                AllResultsQueryConfig.make(
                  ~communityName,
                  ~dateFrom=?dateFrom->Belt.Option.map(toJsonDate),
                  ~dateTo=?dateTo->Belt.Option.map(toJsonDate),
                  (),
                ),
              ),
              ReasonApolloHooks.Utils.toQueryObj(allPlayersQuery),
            |],
        (),
      )
      |> Js.Promise.then_(_ => {
           resetState();
           setIsAddingResult(_ => false) |> Js.Promise.resolve;
         })
      |> Js.Promise.catch(e => {
           Js.Console.error2("Error: ", e);
           setIsAddingResult(_ => false) |> Js.Promise.resolve;
         })
      |> ignore;
    };

  switch (playersQuery) {
  | Loading => <CircularProgress />
  | NoData
  | Error(_) => <span> {text("Error")} </span>
  | Data(data) =>
    <>
      <Paper
        style={ReactDOMRe.Style.make(
          ~width="600px",
          ~marginBottom="30px",
          (),
        )}>
        <div style={ReactDOMRe.Style.make(~display="flex", ())}>
          <PlayerPicker
            disabled=isAddingResult
            placeholderText="Player1"
            playerNames={data##players->Belt.Array.map(p => p##name)}
            selectedPlayerName=maybePlayer1Name
            onChange={v => setMaybePlayer1Name(_ => Some(v))}
          />
          <GoalsPicker
            disabled=isAddingResult
            selectedGoals=goals1
            onChange={v => setGoals1(_ => v)}
          />
          <GoalsPicker
            disabled=isAddingResult
            selectedGoals=goals2
            onChange={v => setGoals2(_ => v)}
          />
          <PlayerPicker
            disabled=isAddingResult
            placeholderText="Player2"
            playerNames={data##players->Belt.Array.map(p => p##name)}
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
          <Button
            disabled=isAddingResult
            variant="contained"
            color="primary"
            onClick=addResult>
            {text("Submit")}
          </Button>
          <FormControlLabel
            control={
              <Checkbox
                disabled=isAddingResult
                color="default"
                checked=extraTime
                onClick=toggleExtraTime
              />
            }
            label="Extra Time"
          />
          <TextField
            disabled=isAddingResult
            _type="date"
            value={formatDate(date)}
            onChange={e => {
              let date =
                Js.Date.fromString(ReactEvent.Form.target(e)##value);
              if (DateFns.isValid(date)) {
                setDate(_ => date);
              };
            }}
          />
        </div>
      </Paper>
    </>
  };
};
