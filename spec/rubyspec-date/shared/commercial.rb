describe :date_commercial, :shared => true do
  ruby_version_is "" ... "1.9" do
    it "creates a Date for the day of Julian calendar reform in Italy by default" do
      d = Date.send(@method)
      d.year.should  == 1582
      d.month.should == 10
      d.day.should   == 15
    end
  end

  ruby_version_is "1.9" do
    it "creates a Date for Julian Day Number day 0 by default" do
      Date.send(@method).jd.should == 0
    end
  end

  ruby_version_is "" ... "1.9" do
    it "Creates a Date for the friday in the year and week given" do
      d = Date.send(@method, 2000, 1)
      d.year.should  == 2000
      d.month.should == 1
      d.day.should   == 7
      d.cwday.should == 5
    end
  end

  ruby_version_is "1.9" do
    it "Creates a Date for the monday in the year and week given" do
      d = Date.send(@method, 2000, 1)
      d.year.should  == 2000
      d.month.should == 1
      d.day.should   == 3
      d.cwday.should == 1
    end
  end

  it "Creates a Date for the correct day given the year, week and day number" do
    d = Date.send(@method, 2004, 1, 1)
    d.year.should   == 2003
    d.month.should  == 12
    d.day.should    == 29
    d.cwday.should  == 1
    d.cweek.should  == 1
    d.cwyear.should == 2004
  end

  it "creates only Date objects for valid weeks" do
    lambda { Date.send(@method, 2004, 53, 1) }.should_not raise_error(ArgumentError)
    lambda { Date.send(@method, 2004, 53, 0) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2004, 53, 8) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2004, 54, 1) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2004,  0, 1) }.should raise_error(ArgumentError)

    lambda { Date.send(@method, 2003, 52, 1) }.should_not raise_error(ArgumentError)
    lambda { Date.send(@method, 2003, 53, 1) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2003, 52, 0) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2003, 52, 8) }.should raise_error(ArgumentError)
  end
end