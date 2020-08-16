open Utils;

module Query = [%relay.query
  {|
    query CommunityStartPageQuery(
      $communityName: String!
      $dateFrom: timestamptz
      $dateTo: timestamptz
    ) {
      results_connection(
        first: 1000
        where: {
          community: { name: { _eq: $communityName } }
          date: { _gte: $dateFrom, _lte: $dateTo }
        }
        order_by: { date: desc }
      )
        @connection(
          key: "CommunityStartPage_query_results_connection"
          filters: []
        ) {
        ...ResultsTable_Results
        ...Stats_Results
        edges {
          node {
            id
          }
        }
      }

      players_connection(
        first: 1000
        where: { community: { name: { _eq: $communityName } } }
      )
        @connection(
          key: "CommunityStartPage_query_players_connection"
          filters: []
        ) {
        ...PlayerPicker_Players
        edges {
          node {
            id
          }
        }
      }

      community_settings_connection(
        where: { community: { name: { _eq: $communityName } } }
      ) {
        edges {
          node {
            ...ResultCommunitySettings_IncludeExtraTime
            ...ResultsTableHeader_CommunitySettings
            ...AddResultFragment_CommunitySettings
            ...StatsTableHeader_ScoreType
          }
        }
      }
    }
  |}
];

[@react.component]
let make = (~communityName) => {
  let (startDate, endDate) = getCurrentWeek();

  let queryData =
    Query.use(
      ~variables={
        communityName,
        dateFrom: Some(startDate),
        dateTo: Some(endDate),
      },
      (),
    );

  let resultsFragment = queryData.results_connection.fragmentRefs;

  let maybeCommunitySettingsFragment =
    queryData.community_settings_connection.edges
    ->Belt.Array.get(0)
    ->Belt.Option.map(e => e.node.fragmentRefs);

  let playersFragment = queryData.players_connection.fragmentRefs;
  <>
    <CommunityStartPageHeader
      communityName
      maybeCommunitySettingsFragment
      playerPickerFragment=playersFragment
    />
    <Stats
      statsResultsFragment=resultsFragment
      maybeScoreTypeFragment=maybeCommunitySettingsFragment
      communityName
      dateFrom=startDate
      dateTo=endDate
    />
    <ResultsTable
      resultsTableFragment=resultsFragment
      maybeCommunitySettingsFragment
      communityName
    />
  </>;
};
