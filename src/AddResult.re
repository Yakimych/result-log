let fakePlayerNames = [|"FakePlayer1", "FakePlayer2"|];

[@react.component]
let make = () => {
  let (isAddingResult, setIsAddingResult) = React.useState(_ => false);
  let (goals1, setGoals1) = React.useState(_ => 0);
  let (goals2, setGoals2) = React.useState(_ => 0);
  let (player1Name, setPlayer1Name) = React.useState(_ => None);
  let (player2Name, setPlayer2Name) = React.useState(_ => None);
  let (extraTime, setExtraTime) = React.useState(_ => false);
  let toggleExtraTime = () => setExtraTime(oldExtraTime => !oldExtraTime);

  let addResult = () => {
    Js.log("Adding result");
  };

  <>
    <Paper
      style={ReactDOMRe.Style.make(~width="550px", ~marginBottom="30px", ())}>
      <div style={ReactDOMRe.Style.make(~display="flex", ())}>
        <PlayerPicker
          disabled=isAddingResult
          placeholderText="Player1"
          playerNames=fakePlayerNames
          selectedPlayerName=player1Name
          onChange={v => setPlayer1Name(_ => Some(v))}
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
          playerNames=fakePlayerNames
          selectedPlayerName=player2Name
          onChange={v => setPlayer2Name(_ => Some(v))}
        />
      </div>
    </Paper>
    <div
      style={ReactDOMRe.Style.make(
        ~display="flex",
        ~justifyContent="space-between",
        (),
      )}>
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
      /> /* TODO: DatePicker */
      <Button
        disabled=isAddingResult
        variant="contained"
        color="primary"
        onClick=addResult>
        {ReasonReact.string("Submit")}
      </Button>
    </div>
  </>;
};
