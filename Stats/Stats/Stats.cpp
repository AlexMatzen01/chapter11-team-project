void Statistics()
{
    Team team;
    Player player;
    int count = 0, total_kills = 0, total_deaths, KD = 0, better_KD = 0;

    while (count < team.playerCount)
    {
        total_kills = total_kills + player.kills;
        total_deaths = total_deaths + player.deaths;
        KD = player.kills / player.deaths;
        if (KD > better_KD)
        {
            better_KD = KD;
        }
    }

}
