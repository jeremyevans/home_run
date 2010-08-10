require File.expand_path('../../spec_helper', __FILE__)

describe :date_civil, :shared => true do
  it "creates a date with arguments" do
    d = Date.send(@method, 2000, 3, 5)
    d.year.should    == 2000
    d.month.should   == 3
    d.day.should     == 5
    d.jd.should      == 2451609

    # Should also work with years far in the past and future

    d = Date.send(@method, -9000, 7, 5)
    d.year.should    == -9000
    d.month.should   == 7
    d.day.should     == 5
    d.jd.should      == -1565937
  
    d = Date.send(@method, 9000, 10, 14)
    d.year.should    == 9000
    d.month.should   == 10
    d.day.should     == 14
    d.jd.should      == 5008529
  
  end

  it "doesn't create dates for invalid arguments" do
    lambda { Date.send(@method, 2000, 13, 31) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2000, 12, 32) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2000,  2, 30) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 1900,  2, 29) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2000,  2, 29) }.should_not raise_error(ArgumentError)  
  end
end


describe "Date#civil" do

  it_behaves_like(:date_civil, :civil)

end

describe "Date#new" do

  it_behaves_like(:date_civil, :new)

end
